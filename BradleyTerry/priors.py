"""
Just a little script for me to fiddle with priors.
This is independent from the rest of the codebase.
"""

import matplotlib.pyplot as plt
import numpy as np
import numpy.random as rng

REPS = 100000

keep = np.empty(REPS)

for i in range(REPS):

    # Hyperparameter
    sig_log_abilities = 2*rng.rand()

    # Generate two abilities
    abilities = np.exp(sig_log_abilities*rng.randn(2))

    # Home team advantage
    h = 1.0 - np.log(rng.rand())
    abilities[0] *= h

    # Probabilities of outcomes
    p = abilities[0]/np.sum(abilities)

    keep[i] = p

print(np.mean(keep))
plt.hist(keep, 500)
plt.show()

