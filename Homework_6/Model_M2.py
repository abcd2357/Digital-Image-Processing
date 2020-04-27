# EE569 Homework Assignment #6
# Problem 2 CIFAR-10 Classification using SSL
# Name: XU Kangyan
# ID: 4876010998
# Email: kangyanx@usc.edu
# Date: Apr 26, 2020

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

    # 10
    X = x_train[0:10]
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
        output[i] = output[i].reshape(10, output[i].shape[1] * output[i].shape[2] * output[i].shape[3])

    # ------------------ cross entropy ------------------ #
    ce = Cross_Entropy(num_class=10, num_bin=5)

    entropy = np.zeros(output[2].shape[1])
    rank = []
    for j in range(0, output[2].shape[1]):
        entropy[j] = ce.KMeans_Cross_Entropy(output[2][:, j].reshape(-1, 1), y_train[0:10])
        rank = np.argsort(-entropy)
    output[2] = output[2][:, rank[0:15]]

    # ------------------ LAG ------------------ #

    lag = LAG(encode = 'distance', num_clusters = [5,5,5,5,5,5,5,5,5,5], alpha = 10, learner = myLLSR(onehot=False))

    # ------------------ Module 3 ------------------#

    print('Time - %2d sec' % (time.time() - start_time))
    print("------- TRAINING FINISHED -------\n")
