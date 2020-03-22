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
B_ori=imread('Puppy_1.jpg');

A=single(rgb2gray(A_ori));
B=single(rgb2gray(B_ori));

% SIFT keypoints and descriptors
% keypoint: center keypoint(1:2), scale keypoint(3) and orientation keypoint(4)
[A_keypoint,A_descriptor]=vl_sift(A,'PeakThresh',PeakThresh,'EdgeThresh',EdgeThresh,'Octaves',Octaves,'Levels',Levels); % Octaves,Levels,EdgeThresh
[B_keypoint,B_descriptor]=vl_sift(B,'PeakThresh',PeakThresh,'EdgeThresh',EdgeThresh,'Octaves',Octaves,'Levels',Levels);

% Basic matching Husky_1 & Husky_3

[matches,scores]=vl_ubcmatch(A_descriptor,B_descriptor);

figure;
imagesc(cat(2,A_ori,B_ori)) ;

x_a=A_keypoint(1,matches(1,:));
x_b=B_keypoint(1,matches(2,:))+size(A,2);
y_a=A_keypoint(2,matches(1,:));
y_b=B_keypoint(2,matches(2,:));

h=line([x_a;x_b],[y_a;y_b]);
set(h,'linewidth',1,'color','y');

vl_plotframe(A_keypoint(:,matches(1,:)));
B_keypoint(1,:)=B_keypoint(1,:)+size(A,2);
vl_plotframe(B_keypoint(:,matches(2,:)));

axis image off;
vl_demo_print('sift_match_A_B');






