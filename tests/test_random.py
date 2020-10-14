import pytest

import dpnp.random
import numpy
# from scipy import stats
from numpy.testing import assert_allclose

@pytest.mark.parametrize("func",
                         [dpnp.random.rand,
                          dpnp.random.randn],
                         ids=['rand', 'randn'])
def test_random_input_size(func):
    output_shape = (10,)
    size = 10
    res = func(size)
    assert output_shape == res.shape


@pytest.mark.parametrize("func",
                         [dpnp.random.random,
                          dpnp.random.random_sample,
                          dpnp.random.randf,
                          dpnp.random.sample],
                         ids=['random', 'random_sample',
                              'randf', 'sample'])
def test_random_input_shape(func):
    shape = (10, 5)
    res = func(shape)
    assert shape == res.shape


@pytest.mark.parametrize("func",
                         [dpnp.random.random,
                          dpnp.random.random_sample,
                          dpnp.random.randf,
                          dpnp.random.sample,
                          dpnp.random.rand],
                         ids=['random', 'random_sample',
                              'randf', 'sample',
                              'rand'])
def test_random_check_otput(func):
    shape = (10, 5)
    size = 10 * 5
    if func == dpnp.random.rand:
        res = func(size)
    else:
        res = func(shape)
#    assert numpy.all(res >= 0)
#    assert numpy.all(res < 1)
    for i in range(res.size):
        assert res[i] >= 0.0
        assert res[i] < 1.0


def test_randn_normal_distribution():
    """ Check if the sample obtained from the dpnp.random.randn differs from
    the normal distribution.
    Using ``scipy.stats.normaltest``.

    It is based on D’Agostino and Pearson’s test that combines skew
    and kurtosis to produce an omnibus test of normality,
    see: https://docs.scipy.org/doc/scipy/reference/generated/scipy.stats.normaltest.html
    """
    pts = 1000
    alpha = 0.05
    dpnp.random.seed(28041990)
    x = dpnp.random.randn(pts)
    _, p = stats.normaltest(x)
    # null hypothesis: x comes from a normal distribution.
    # The p-value is interpreted against an alpha of 5% and finds that the test
    # dataset does not significantly deviate from normal.
    # If p > alpha, the null hypothesis cannot be rejected.
    assert p > alpha


@pytest.mark.parametrize("func",
                         [dpnp.random.random,
                          dpnp.random.random_sample,
                          dpnp.random.randf,
                          dpnp.random.sample,
                          dpnp.random.rand],
                         ids=['random', 'random_sample',
                              'randf', 'sample',
                              'rand'])
def test_radnom_seed(func):
    seed = 28041990
    size = 100
    shape = (100, 1)
    if func in [dpnp.random.rand, dpnp.random.randn]:
        args = size
    else:
        args = shape
    dpnp.random.seed(seed)
    a1 = func(args)
    dpnp.random.seed(seed)
    a2 = func(args)
    assert_allclose(a1, a2, rtol=1e-07, atol=0)
