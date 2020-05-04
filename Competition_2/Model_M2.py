# EE569 Competition #2
# Problem 3 CIFAR-10 Classification using SSL
# Name: XU Kangyan
# ID: 4876010998
# Email: kangyanx@usc.edu
# Date: May 3, 2020

import numpy as np
import sklearn
import time
from skimage.util import view_as_windows
import skimage.measure
from keras.datasets import cifar10
from cross_entropy import Cross_Entropy
from lag import LAG
import pickle
from llsr import LLSR as myLLSR
from sklearn.metrics import confusion_matrix
from sklearn.ensemble import RandomForestClassifier


if __name__ == "__main__":

    # ------------------ Module 1 ------------------#

    # callback function for collecting patches and its inverse
    def Shrink(X, shrinkArg):
        win = shrinkArg['win']
        stride = shrinkArg['stride']
        module = shrinkArg['Module']
        ch = X.shape[-1]
        # max pooling
        if module != 1:
            X = skimage.measure.block_reduce(X, (1, 2, 2, 1), np.max)
        X = view_as_windows(X, (1, win, win, ch), (1, stride, stride, ch))
        return X.reshape(X.shape[0], X.shape[1], X.shape[2], -1)

    # callback function for how to concat features from different hops
    def Concat(X, concatArg):
        return X

    start_time = time.time()

    # read data
    (x_train, y_train), (x_test, y_test) = cifar10.load_data()
    x_train = x_train.astype('float32')
    x_train /= 255
    x_test = x_test.astype('float32')
    x_test /= 255

    # set args
    SaabArgs = [{'num_AC_kernels': -1, 'needBias': False, 'useDC': True, 'cw': False},
                {'num_AC_kernels': -1, 'needBias': True, 'useDC': True, 'cw': True},
                {'num_AC_kernels': -1, 'needBias': True, 'useDC': True, 'cw': True}]
    shrinkArgs = [{'func': Shrink, 'win': 5, 'stride': 1, 'Module': 1},
                  {'func': Shrink, 'win': 5, 'stride': 1, 'Module': 2},
                  {'func': Shrink, 'win': 5, 'stride': 1, 'Module': 3}]
    concatArg = {'func': Concat}

    # ------------------ Module 2 ------------------#

    # ------------------ Parameter ------------------#
    train_num = 3500
    test_num = 3500
    NoC = 6
    Ns_1 = 528
    Ns_2 = 528
    Ns_3 = 528
    # -----------------------------------------------#

    # ------------------ Training Data ------------------ #
    X = x_train[0:train_num]
    print("input feature shape: %s" % str(X.shape))

    with open('PixelHop.pickle', 'rb') as file:
        new_p2 = pickle.load(file)

    output = new_p2.transform(X)
    print(output[0].shape, output[1].shape, output[2].shape)

    # max pooling
    for i in range(0, 3):
        output[i] = skimage.measure.block_reduce(output[i], (1, 2, 2, 1), np.max)

    # reshaping
    for i in range(0, 3):
        output[i] = output[i].reshape(train_num, output[i].shape[1] * output[i].shape[2] * output[i].shape[3])

    # ------------------ cross entropy ------------------ #
    ce = Cross_Entropy(num_class=10, num_bin=5)

    print('Ns_1 - %d Ns_2 - %d Ns_3 - %d' % (Ns_1, Ns_2, Ns_3))

    entropy = np.zeros(output[0].shape[1])
    rank_1 = []
    for j in range(0, output[0].shape[1]):
        entropy[j] = ce.compute(output[0][:, j].reshape(-1, 1), y_train[0:train_num])
        rank_1 = np.argsort(-entropy)
    output[0] = output[0][:, rank_1[0:Ns_1]]

    # ------------------------------------ #

    entropy = np.zeros(output[1].shape[1])
    rank_2 = []
    for j in range(0, output[1].shape[1]):
        entropy[j] = ce.compute(output[1][:, j].reshape(-1, 1), y_train[0:train_num])
        rank_2 = np.argsort(-entropy)
    output[1] = output[1][:, rank_2[0:Ns_2]]

    # ------------------------------------ #

    entropy = np.zeros(output[2].shape[1])
    rank_3 = []
    for j in range(0, output[2].shape[1]):
        entropy[j] = ce.compute(output[2][:, j].reshape(-1, 1), y_train[0:train_num])
        rank_3 = np.argsort(-entropy)
    output[2] = output[2][:, rank_3[0:Ns_3]]

    # ------------------ LAG ------------------ #

    print('NoC - %d' % NoC)

    lag_1 = LAG(encode='distance', num_clusters=[NoC, NoC, NoC, NoC, NoC, NoC, NoC, NoC, NoC, NoC], alpha=10,
                learner=myLLSR(onehot=False))
    lag_2 = LAG(encode='distance', num_clusters=[NoC, NoC, NoC, NoC, NoC, NoC, NoC, NoC, NoC, NoC], alpha=10,
                learner=myLLSR(onehot=False))
    lag_3 = LAG(encode='distance', num_clusters=[NoC, NoC, NoC, NoC, NoC, NoC, NoC, NoC, NoC, NoC], alpha=10,
                learner=myLLSR(onehot=False))

    lag_1.fit(output[0], y_train[0:train_num])
    output[0] = lag_1.transform(output[0])

    lag_2.fit(output[1], y_train[0:train_num])
    output[1] = lag_2.transform(output[1])

    lag_3.fit(output[2], y_train[0:train_num])
    output[2] = lag_3.transform(output[2])

    # ------------------ Module 3 ------------------#

    feature_vector = np.zeros([train_num, 3*NoC*10])
    feature_vector[:, 0:NoC*10] = output[0]
    feature_vector[:, NoC * 10:2*NoC*10] = output[1]
    feature_vector[:, 2*NoC*10:3*NoC*10] = output[2]

    # ------------------ Random Forest ------------------#

    # Training classifier
    classifier = RandomForestClassifier(n_estimators=100)
    classifier.fit(feature_vector, y_train[0:train_num])

    print('Time - %2d sec' % (time.time() - start_time))
    print("------- TRAINING FINISHED -------\n")

    # ------------------ Testing ------------------#
    test_start_time = time.time()

    # ------------------ Test Data ------------------ #
    T = x_test[0:test_num]
    print("input feature shape: %s" % str(T.shape))

    with open('PixelHop.pickle', 'rb') as file:
        new_p2 = pickle.load(file)

    T_output = new_p2.transform(T)
    print(T_output[0].shape, T_output[1].shape, T_output[2].shape)

    # max pooling
    for i in range(0, 3):
        T_output[i] = skimage.measure.block_reduce(T_output[i], (1, 2, 2, 1), np.max)

    # reshaping
    for i in range(0, 3):
        T_output[i] = T_output[i].reshape(test_num, T_output[i].shape[1] * T_output[i].shape[2] * T_output[i].shape[3])

    # ------------------ LAG ------------------ #

    T_output[0] = T_output[0][:, rank_1[0:Ns_1]]
    T_output[1] = T_output[1][:, rank_2[0:Ns_2]]
    T_output[2] = T_output[2][:, rank_3[0:Ns_3]]

    T_output[0] = lag_1.transform(T_output[0])
    T_output[1] = lag_2.transform(T_output[1])
    T_output[2] = lag_3.transform(T_output[2])

    # ------------------ Module 3 ------------------#

    feature_vector = np.zeros([test_num, 3 * NoC * 10])
    feature_vector[:, 0:NoC * 10] = T_output[0]
    feature_vector[:, NoC * 10:2 * NoC * 10] = T_output[1]
    feature_vector[:, 2 * NoC * 10:3 * NoC * 10] = T_output[2]

    # ------------------ Random Forest ------------------#

    predicted = classifier.predict(feature_vector)

    test_predict = predicted.reshape(-1, 1)
    test_label = y_test[0:test_num].reshape(-1, 1)

    correct = 0
    correct += (test_predict == test_label).sum().item()

    test_accuracy = 100 * correct / test_num
    print('Accuracy on 3500 test images: %.3f %%' % test_accuracy)
    print('Time - %2d sec' % (time.time() - test_start_time))
    print("------- TESTING FINISHED -------\n")
