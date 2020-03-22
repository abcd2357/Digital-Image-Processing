% EE569 Homework Assignment #4
% Problem 1(b) Texture Classification --- Classifier Explore 2
% Name: XU Kangyan
% ID: 4876010998
% Email: kangyanx@usc.edu
% Date: Mar 22, 2020
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

load('Test_3D_Feature.mat')
TestFeature3D=Yr;
load('Train_3D_Feature.mat');
TrainFeature3D=Yr;

Class_label=[0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3]';

% Train Random Forest
RF_Model=TreeBagger(100,TrainFeature3D,Class_label,'Method','classification');
RF_Predict_label=predict(RF_Model,TestFeature3D);

% Train Support Vector Machine
t=templateSVM('Standardize',1,'KernelFunction','gaussian'); % SVM template, standardize the predictor data, Radial Basis Function (RBF) Kernel
SVM_Model=fitcecoc(TrainFeature3D,Class_label,'Learners',t);
SVM_error=resubLoss(SVM_Model); % resubstitution classification error
SVM_Predict_label=predict(SVM_Model,TestFeature3D);