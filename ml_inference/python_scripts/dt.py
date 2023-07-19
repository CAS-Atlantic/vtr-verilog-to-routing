import numpy as np
import warnings
from joblib import load
warnings.filterwarnings("ignore")
def get_optimal_ratio():
	clf_dt = load('../trained_models/dt_saved.joblib')
	ratio_dict = {4: 0.75, 1: 0.2, 8: 1.0, 3: 0.7, 0: 0.0, 2: 0.25, 6: 0.9, 7: 0.95, 5: 0.85}
	x = [0,0,0,0]
	with open ('../../vtr_flow/scripts/temp/netstats.txt', 'r') as s:
		for i in s:
			if "Memory" in i:
				j = i.split(": ")
				x[0]=(int(float(j[-1])))
			if "MULTIPLY" in i:
				j = i.split(": ")
				x[1]=(int(float(j[-1])))
			if "lut" in i:
				j = i.split(": ")
				x[2]=(int(float(j[-1])))
			if "Longest" in i:
				j = i.split(": ")
				x[3]=(int(float(j[-1])))
	X_test = np.array([x])
	Y_pred_gnb = clf_dt.predict(X_test)
	optimal_ratio = ratio_dict[Y_pred_gnb[0]]
	print(optimal_ratio)
	return optimal_ratio
result = get_optimal_ratio()