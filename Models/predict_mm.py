#!/usr/bin/env python

import numpy as np
import numpy.random as rng
import pandas as pd
import sys

posterior_samples = pd.read_csv("output/posterior.csv")

teams = sys.argv[1:]
if len(teams) != 2:
    print("Usage: ./predict.py <home_team> <away_team>")
    sys.exit(0)

home_advantage = posterior_samples["home_advantage"]
C = posterior_samples["C"]
W = posterior_samples["W"]
abilities = np.array([posterior_samples[f"abilities[{sys.argv[1]}]"],
                      posterior_samples[f"abilities[{sys.argv[2]}]"]]).T
log_ability_ratio = np.log(home_advantage*abilities[:,0]) \
                        - np.log(abilities[:,1])
mu = C*log_ability_ratio
es = -np.log(1.0 - rng.rand(posterior_samples.shape[0]))
es[rng.rand(posterior_samples.shape[0]) < 0.5] *= -1
margins = mu + W*es
prob = np.mean(margins > 0.0)

print(f"Probabilities = ({prob}, {1.0 - prob}).")

