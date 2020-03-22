% EE569 Homework Assignment #4
% Problem 2(c) Bag of Words Feature
% Name: XU Kangyan
% ID: 4876010998
% Email: kangyanx@usc.edu
% Date: Mar 22, 2020
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% VLFeat Reference: https://www.vlfeat.org/overview/sift.html

%---------------------------%
% Parameter
PeakThresh=6; % value below this threshold can not become a keypoint.
EdgeThresh=12.1; % Increase lead to a larger number of keypoints sensivite to noise. (r+1)^2/r
Octaves=4;
Levels=5;
%---------------------------%

A_ori=imread('Husky_1.jpg');
B_ori=imread('Husky_2.jpg');
C_ori=imread('Husky_3.jpg');
D_ori=imread('Puppy_1.jpg');

A=single(rgb2gray(A_ori));
B=single(rgb2gray(B_ori));
C=single(rgb2gray(C_ori));
D=single(rgb2gray(D_ori));

% SIFT keypoints and descriptors
% keypoint: center keypoint(1:2), scale keypoint(3) and orientation keypoint(4)
[A_keypoint,A_descriptor]=vl_sift(A,'PeakThresh',PeakThresh,'EdgeThresh',EdgeThresh,'Octaves',Octaves,'Levels',Levels);

% nearest neighbor search
[fix_A,num_A]=size(A_descriptor);
A_Feature=zeros(128,num_A);

for i=1:num_A
    vector_2=A_descriptor(:,i);
    % Normalization
    vector_2=double(vector_2)/sum(vector_2);
    % 0.2 Threshold Cap
    for j=1:128
        if vector_2(j,1)>0.2
            vector_2(j,1)=0.2;
        end
    end
    % Normalization Again
    vector_2=double(vector_2)/sum(vector_2);
    A_Feature(:,i)=vector_2;
end

A_Feature=A_Feature';
%------------------------------------------------

[B_keypoint,B_descriptor]=vl_sift(B,'PeakThresh',PeakThresh,'EdgeThresh',EdgeThresh,'Octaves',Octaves,'Levels',Levels);

% nearest neighbor search
[fix_B,num_B]=size(B_descriptor);
B_Feature=zeros(128,num_B);

for i=1:num_B
    vector_2=B_descriptor(:,i);
    % Normalization
    vector_2=double(vector_2)/sum(vector_2);
    % 0.2 Threshold Cap
    for j=1:128
        if vector_2(j,1)>0.2
            vector_2(j,1)=0.2;
        end
    end
    % Normalization Again
    vector_2=double(vector_2)/sum(vector_2);
    B_Feature(:,i)=vector_2;
end

B_Feature=B_Feature';
%------------------------------------------------

[C_keypoint,C_descriptor]=vl_sift(C,'PeakThresh',PeakThresh,'EdgeThresh',EdgeThresh,'Octaves',Octaves,'Levels',Levels);

% nearest neighbor search
[fix_C,num_C]=size(C_descriptor);
C_Feature=zeros(128,num_C);

for i=1:num_C
    vector_2=C_descriptor(:,i);
    % Normalization
    vector_2=double(vector_2)/sum(vector_2);
    % 0.2 Threshold Cap
    for j=1:128
        if vector_2(j,1)>0.2
            vector_2(j,1)=0.2;
        end
    end
    % Normalization Again
    vector_2=double(vector_2)/sum(vector_2);
    C_Feature(:,i)=vector_2;
end

C_Feature=C_Feature';
%------------------------------------------------

[D_keypoint,D_descriptor]=vl_sift(D,'PeakThresh',PeakThresh,'EdgeThresh',EdgeThresh,'Octaves',Octaves,'Levels',Levels);

% nearest neighbor search
[fix_D,num_D]=size(D_descriptor);
D_Feature=zeros(128,num_D);

for i=1:num_D
    vector_2=D_descriptor(:,i);
    % Normalization
    vector_2=double(vector_2)/sum(vector_2);
    % 0.2 Threshold Cap
    for j=1:128
        if vector_2(j,1)>0.2
            vector_2(j,1)=0.2;
        end
    end
    % Normalization Again
    vector_2=double(vector_2)/sum(vector_2);
    D_Feature(:,i)=vector_2;
end

D_Feature=D_Feature';











