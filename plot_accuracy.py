import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt


def getTrainTuneTestErrors(input_fname, input_ext):
    input_file = open(input_fname + input_ext)

    lines = input_file.readlines()

    prefix = 'Predicted'

    index = 0

    train_accuracies = []
    tune_accuracies = []
    test_accuracies = []

    for line in lines:
        if line.startswith(prefix):
            tokens = line.split(':')
            accuracy = float(tokens[1])
            if index % 3 == 0:
                train_accuracies.append(accuracy)
            elif index % 3 == 1:
                tune_accuracies.append(accuracy)
            else:
                test_accuracies.append(accuracy)

            index += 1

    print len(train_accuracies), len(tune_accuracies), len(test_accuracies)

    train_accuracies = np.array(train_accuracies)
    tune_accuracies = np.array(tune_accuracies)
    test_accuracies = np.array(test_accuracies)

    train_errors = np.subtract(1.0, train_accuracies)
    tune_errors = np.subtract(1.0, tune_accuracies)
    test_errors = np.subtract(1.0, test_accuracies)

    return train_errors, tune_errors, test_errors



input_fname_0 = '/u/a/d/adbhat/private/gitRepository/CS838Lab3/results/results_weight_sharing_augmented'
input_fname_1 = '/u/a/d/adbhat/private/gitRepository/CS838Lab3/resultsresults_weight_sharing2'

input_ext = '.txt'
output_ext = '.png'

train_errors_0, tune_errors_0, test_errors_0 =  getTrainTuneTestErrors(input_fname=input_fname_0, input_ext=input_ext)
train_errors_1, tune_errors_1, test_errors_1 =  getTrainTuneTestErrors(input_fname=input_fname_1, input_ext=input_ext)

epochs = range(len(train_errors_0) - 1)

sns.set(style="darkgrid")
# plt.plot(epochs, train_errors[:-1])
plt.plot(epochs, test_errors_0[:-1])
# plt.plot(epochs, tune_errors[:-1])
plt.xlabel('epoch')
plt.ylabel('error')
plt.legend(['With Augmentation', 'Without Augmentation'])
plt.savefig(input_fname_0 + 'comparison'+ output_ext, bbox_inches='tight', pad_inches=0)