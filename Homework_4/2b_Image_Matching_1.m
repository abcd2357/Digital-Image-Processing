% EE569 Homework Assignment #4
% Problem 2(b) Image Matching
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
[C_keypoint,C_descriptor]=vl_sift(C,'PeakThresh',PeakThresh,'EdgeThresh',EdgeThresh,'Octaves',Octaves,'Levels',Levels); % Octaves,Levels,EdgeThresh

[scale_max,index]=max(C_keypoint(3,:));
C_feature_vector=C_descriptor(:,index);

% Normalization
C_feature_vector=double(C_feature_vector)/sum(C_feature_vector);
% 0.2 Threshold Cap
for i=1:128
    if C_feature_vector(i,1)>=0.2
        C_feature_vector(i,1)=0.2;
    end
end
% Normalization Again
C_feature_vector=double(C_feature_vector)/sum(C_feature_vector);

figure;
imagesc(C);

% Display the Extracting Keypoint & Descriptor with Largest Scale

h1=vl_plotframe(C_keypoint(:,index));
set(h1,'color','k','linewidth',3) ;
h2=vl_plotframe(C_keypoint(:,index));
set(h2,'color','y','linewidth',2) ;
h3=vl_plotsiftdescriptor(C_descriptor(:,index),C_keypoint(:,index));
set(h3,'color','k','linewidth',2);
h4=vl_plotsiftdescriptor(C_descriptor(:,index),C_keypoint(:,index));
set(h4,'color','g','linewidth',1);

colormap gray;
axis equal;axis off;axis tight;
vl_demo_print('Husky_3_scale_largest');

%---------------------------------%

[A_keypoint,A_descriptor]=vl_sift(A,'PeakThresh',PeakThresh,'EdgeThresh',EdgeThresh,'Octaves',Octaves,'Levels',Levels);

% nearest neighbor search
[fix,num]=size(A_descriptor);

distance=100;
nearest_index=1;
dist_record=zeros(128,1);

for i=1:num
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
    
    % nearest neighbor search L2-Norm / Euclidean
    temp=norm(vector_2-C_feature_vector);
    dist_record(i,1)=temp;
    
    if temp<distance
        distance=temp;
        nearest_index=i;
        nearest_feature_vector=vector_2;
    end
end

figure;
imagesc(A);

h1=vl_plotframe(A_keypoint(:,nearest_index));
set(h1,'color','k','linewidth',3) ;
h2=vl_plotframe(A_keypoint(:,nearest_index));
set(h2,'color','y','linewidth',2) ;
h3=vl_plotsiftdescriptor(A_descriptor(:,nearest_index),A_keypoint(:,nearest_index));
set(h3,'color','k','linewidth',2);
h4=vl_plotsiftdescriptor(A_descriptor(:,nearest_index),A_keypoint(:,nearest_index));
set(h4,'color','g','linewidth',1);

colormap gray;
axis equal;axis off;axis tight;
vl_demo_print('Husky_1_nearest');

%---------------------------------%

% Basic matching Husky_1 & Husky_3

% A_keypoint A_descriptor
% C_keypoint C_descriptor

[matches,scores]=vl_ubcmatch(A_descriptor,C_descriptor);

figure;
imagesc(cat(2,A_ori,C_ori)) ;
range=1:10;

x_a=A_keypoint(1,matches(1,range));
x_c=C_keypoint(1,matches(2,range))+size(A,2);
y_a=A_keypoint(2,matches(1,range));
y_c=C_keypoint(2,matches(2,range));

h=line([x_a;x_c],[y_a;y_c]);
set(h,'linewidth',1,'color','y');

vl_plotframe(A_keypoint(:,matches(1,range)));
C_keypoint(1,:)=C_keypoint(1,:)+size(A,2);
vl_plotframe(C_keypoint(:,matches(2,range)));

axis image off;
vl_demo_print('sift_match_1_3');














