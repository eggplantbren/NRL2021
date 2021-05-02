#!/usr/bin/env python

import numpy as np
import pandas as pd
import sys

posterior_samples = pd.read_csv("output/posterior.csv")

teams = sys.argv[1:]
if len(teams) != 2:
    print("Usage: ./predict.py <home_team> <away_team>")
    sys.exit(0)

home_advantage = posterior_samples["home_advantage"]
abilities = np.array([posterior_samples[f"abilities[{sys.argv[1]}]"],
                      posterior_samples[f"abilities[{sys.argv[2]}]"]]).T
probs = home_advantage*abilities[:,0] / \
            (home_advantage*abilities[:,0] + abilities[:,1])
prob = np.mean(probs)
print(f"Probabilities = ({prob}, {1.0 - prob}).")

