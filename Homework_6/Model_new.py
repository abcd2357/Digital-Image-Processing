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
from pixelhop2 import Pixelhop2
from sklearn.metrics import confusion_matrix
from sklearn.externals import joblib


if __name__ == "__main__":

    # Training 1w images

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

    X = x_train[0:10000]
    print("input feature shape: %s" % str(X.shape))

    # set args
    SaabArgs = [{'num_AC_kernels': -1, 'needBias': False, 'useDC': True, 'cw': False},
                {'num_AC_kernels': -1, 'needBias': True, 'useDC': True, 'cw': True},
                {'num_AC_kernels': -1, 'needBias': True, 'useDC': True, 'cw': True}]
    shrinkArgs = [{'func': Shrink, 'win': 5, 'stride': 1, 'Module': 1},
                  {'func': Shrink, 'win': 5, 'stride': 1, 'Module': 2},
                  {'func': Shrink, 'win': 5, 'stride': 1, 'Module': 3}]
    concatArg = {'func': Concat}

    p2 = Pixelhop2(depth=3, TH1=0.001, TH2=0.0001, SaabArgs=SaabArgs, shrinkArgs=shrinkArgs, concatArg=concatArg)
    p2.fit(X)
    output = p2.transform(X)
    # joblib.dump(p2, "Module_1.m")
    print(output[0].shape, output[1].shape, output[2].shape)

    with open('PixelHop.pickle', 'wb') as file:
        pickle.dump(p2, file)

    print('Time - %2d sec' % (time.time() - start_time))
    print("------- TRAINING FINISHED -------\n")
