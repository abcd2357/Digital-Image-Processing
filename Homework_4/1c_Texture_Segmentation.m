% EE569 Homework Assignment #4
% Problem 1(c) Texture Segmentation
% Name: XU Kangyan
% ID: 4876010998
% Email: kangyanx@usc.edu
% Date: Mar 22, 2020
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% writerawNS(image, 'X.raw');

L5 = [1;4;6;4;1];
E5 = [-1;-2;0;2;1];
S5 = [-1;0;2;0;-1];
W5 = [-1;2;0;-2;1];
R5 = [1;-4;6;-4;1];

w=600;
h=450;

image=readrawNS('Comp.raw',w,h);
%writerawNS(image,'Test_1.raw');

% Subtract image mean (reduce illumination effects) 

image_mean = sum(image(:))/(w*h);
for m=1:h
    for n=1:w
        image(m,n) = image(m,n) - image_mean;
    end
end

%writerawNS(image,'Test_2.raw');

Laws_filter = cell(1,25);
Laws_filter{1} = L5*L5';Laws_filter{2} = L5*E5';Laws_filter{3} = L5*S5';Laws_filter{4} = L5*W5';Laws_filter{5} = L5*R5';
Laws_filter{6} = E5*L5';Laws_filter{7} = E5*E5';Laws_filter{8} = E5*S5';Laws_filter{9} = E5*W5';Laws_filter{10} = E5*R5';
Laws_filter{11} = S5*L5';Laws_filter{12} = S5*E5';Laws_filter{13} = S5*S5';Laws_filter{14} = S5*W5';Laws_filter{15} = S5*R5';
Laws_filter{16} = W5*L5';Laws_filter{17} = W5*E5';Laws_filter{18} = W5*S5';Laws_filter{19} = W5*W5';Laws_filter{20} = W5*R5';
Laws_filter{21} = R5*L5';Laws_filter{22} = R5*E5';Laws_filter{23} = R5*S5';Laws_filter{24} = R5*W5';Laws_filter{25} = R5*R5';

% Apply 25 5x5 Laws filters on each texture image to get 25 filtered images
feature_image=cell(25,1);
        
% Mirror Reflecting Padding 600+2+2 450+2+2
temp=zeros(h+4,w+4);
for m=1:h
    for n=1:w
        temp(m+2,n+2) = image(m,n);
    end
end
        
for n=3:h+2   
    temp(n,2) = temp(n,3);
    temp(n,1) = temp(n,4); 
end
            
for n=1:w+2    
    temp(1,n) = temp(4,n);
    temp(2,n) = temp(3,n); 
end
            
for n=1:h+2    
    temp(n,w+3) = temp(n,w+2);
    temp(n,w+4) = temp(n,w+1); 
end
            
for n=1:w+4   
    temp(h+3,n) = temp(h+2,n);
    temp(h+4,n) = temp(h+1,n); 
end
        
% Get 25 filtered images & 25-D feature vector 
       
for k=1:25
            
    temp_2=zeros(h,w);
            
    for m=1:h
        for n=1:w
            for x=-2:2
                for y=-2:2
                    temp_2(m,n)=temp_2(m,n)+Laws_filter{1,k}(3+x,3+y)*temp(m+2+x,n+2+y);
                end
            end
        end
    end
            
    feature_image{k,1}=temp_2;
end

% Use a window to compute energy for each pixel on each dimension 

%****************************%
windowsize=13;
%****************************%

ws=int16((windowsize-1)/2);
window=zeros(windowsize);

feature_vector_25D=cell(h,w);
temp_3=cell(25,1);

% Mirror Reflecting Padding

for k=1:25
    temp_3{k,1}=zeros(h+2*ws,w+2*ws);
    for i=1:h
        for j=1:w
            temp_3{k,1}(i+ws,j+ws)=feature_image{k,1}(i,j);
        end
    end
    
    for i=1:ws
        temp_3{k,1}(1:h+windowsize-1,i)=temp_3{k,1}(1:h+windowsize-1,windowsize-i);
    end
    
    for i=1:ws
        temp_3{k,1}(i,1:w+windowsize-1)=temp_3{k,1}(windowsize-i,1:w+windowsize-1);
    end
    
    for i=1:ws
        temp_3{k,1}(1:h+windowsize-1,w+ws+i)=temp_3{k,1}(1:h+windowsize-1,w+windowsize-i-ws);
    end
    
     for i=1:ws
        temp_3{k,1}(h+ws+i,1:w+windowsize-1)=temp_3{k,1}(h+windowsize-i-ws,1:w+windowsize-1);
    end
    
end


for i=1:h
    for j=1:w
        for k=1:25
            window=abs(temp_3{k,1}(i:i+2*ws,j:j+2*ws));
            feature_vector_25D{i,j}(k,1)=sum(window(:))/(windowsize*windowsize);
        end
    end 
end

% 14-D feature vector L5E5/E5L5 E5S5/S5E5 E5E5 L5S5/S5L5 E5W5/W5E5 S5S5 L5W5/W5L5 E5R5/R5E5 W5W5 L5R5/R5L5 S5W5/W5S5 R5R5 W5R5/R5W5 S5R5/R5S5
% normalization with L5L5
feature_vector_14D=zeros(270000,14);

for i=1:h
    for j=1:w
        normalization_k=feature_vector_25D{i,j}(1,1); % L5L5
        index=w*(i-1)+j;
        feature_vector_14D(index,1)=(feature_vector_25D{i,j}(2,1)+feature_vector_25D{i,j}(6,1))/(2*normalization_k); % L5E5/E5L5
        feature_vector_14D(index,2)=(feature_vector_25D{i,j}(8,1)+feature_vector_25D{i,j}(12,1))/(2*normalization_k); % E5S5/S5E5
        feature_vector_14D(index,3)=feature_vector_25D{i,j}(7,1)/normalization_k; % E5E5
        feature_vector_14D(index,4)=(feature_vector_25D{i,j}(3,1)+feature_vector_25D{i,j}(11,1))/(2*normalization_k); % L5S5/S5L5
        feature_vector_14D(index,5)=(feature_vector_25D{i,j}(9,1)+feature_vector_25D{i,j}(17,1))/(2*normalization_k); % E5W5/W5E5
        feature_vector_14D(index,6)=feature_vector_25D{i,j}(13,1)/normalization_k; % S5S5
        feature_vector_14D(index,7)=(feature_vector_25D{i,j}(4,1)+feature_vector_25D{i,j}(16,1))/(2*normalization_k); % L5W5/W5L5
        feature_vector_14D(index,8)=(feature_vector_25D{i,j}(10,1)+feature_vector_25D{i,j}(22,1))/(2*normalization_k); % E5R5/R5E5
        feature_vector_14D(index,9)=feature_vector_25D{i,j}(19,1)/normalization_k; % W5W5
        feature_vector_14D(index,10)=(feature_vector_25D{i,j}(5,1)+feature_vector_25D{i,j}(21,1))/(2*normalization_k); % L5R5/R5L5
        feature_vector_14D(index,11)=(feature_vector_25D{i,j}(14,1)+feature_vector_25D{i,j}(18,1))/(2*normalization_k); % S5W5/W5S5
        feature_vector_14D(index,12)=feature_vector_25D{i,j}(25,1)/normalization_k; % R5R5
        feature_vector_14D(index,13)=(feature_vector_25D{i,j}(20,1)+feature_vector_25D{i,j}(24,1))/(2*normalization_k); % W5R5/R5W5
        feature_vector_14D(index,14)=(feature_vector_25D{i,j}(15,1)+feature_vector_25D{i,j}(23,1))/(2*normalization_k); % S5R5/R5S5
    end
end

%{
% K-means with K=6
idx_1 = kmeans(feature_vector_14D,6,'Start','uniform');

% Output 0 51 102 153 204 255
Image_out=zeros(h,w);
for i=1:h
    for j=1:w
        Image_out(i,j)=(idx_1(w*(i-1)+j,1)-1)*51;
    end
end

%****************************%
writerawNS(Image_out, 'Texture_Segmentation_14D.raw');
%}

%*************************************************************************%

% Feature Reduction - PCA

Y=zeros(270000,14);
for j=1:14
    mean_Xj=sum(feature_vector_14D(:,j))/270000;
    Y(:,j)=feature_vector_14D(:,j)-mean_Xj;
end

% Singular Value Decomposition

s=svds(Y); % e-value
[U,S,V]=svds(Y);
Vr=V(:,1:3); % e-vector
Yr=Y*Vr; % 270000,3 feature_vector_3D

% K-means with K=6
idx_2 = kmeans(Yr,6,'Start','uniform');

% Output 0 51 102 153 204 255
Image_out_2=zeros(h,w);
for i=1:h
    for j=1:w
        Image_out_2(i,j)=(idx_2(w*(i-1)+j,1)-1)*51;
    end
end

writerawNS(Image_out_2, 'Texture_Segmentation_3D.raw');





















