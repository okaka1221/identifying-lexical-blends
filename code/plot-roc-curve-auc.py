import pandas as pd
from sklearn.metrics import roc_curve, auc
import matplotlib.pyplot as plt

methods = ["Jaro-Winkler Similarity", "Local Edit Distance", "Global Edit Distance", "2-Gram Distance"]

# define file path
path = {
    "Local Edit Distance": "../output/local-edit-distance",
    "Jaro-Winkler Similarity": "../output/jaro-winkler",
    "Global Edit Distance": "../output/global-edit-distance",
    "2-Gram Distance": "../output/2-gram",
}

# calculate auc score and plot roc curve
for method in methods:
    train = pd.read_csv(path[method] + ".csv")
    test = pd.read_csv("../output/test.csv")
    fpr, tpr, thresholds = roc_curve(test['label'], train['score'], pos_label=1)
    roc_auc = auc(fpr, tpr)
    print("Method: %s, AUC = %0.4f" % (method, roc_auc))
    plt.plot(fpr, tpr, label="%s (AUC = %0.2f)" % (method, roc_auc))

# add roc curve of uniform distribution to plot   
plt.plot([0,1], [0,1], linestyle='--', color=[0.6,0.6,0.6])
# style graph
plt.xlim([0, 1])
plt.ylim([0, 1])
plt.xlabel('FPR', fontsize=16)
plt.ylabel('TPR', fontsize=16)
plt.legend(loc='lower right', fontsize=12)
plt.tight_layout()
plt.savefig('../output/roc-curve.png')
plt.show()