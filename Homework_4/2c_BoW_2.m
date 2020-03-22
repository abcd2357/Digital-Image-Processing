% EE569 Homework Assignment #4
% Problem 2(c) Bag of Words 2
% Name: XU Kangyan
% ID: 4876010998
% Email: kangyanx@usc.edu
% Date: Mar 22, 2020
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

load('A_Feature.mat')
load('B_Feature.mat')
load('C_Feature.mat')
load('D_Feature.mat')

[idx_A,Centroid_A]=kmeans(A_Feature,8,'Start','uniform');
[idx_B,Centroid_B]=kmeans(B_Feature,8,'Start','uniform');
[idx_C,Centroid_C]=kmeans(C_Feature,8,'Start','uniform');
[idx_D,Centroid_D]=kmeans(D_Feature,8,'Start','uniform');

%-----------------------------
dist_AC=zeros(8,8);
Nearest_C_for_A=zeros(1,8);

for i=1:8
    for j=1:8
        dist_AC(i,j)=dist(Centroid_A(i,:),Centroid_C(j,:)');
    end
end

for i=1:8
    [nearest,index]=min(dist_AC(i,:));
    Nearest_C_for_A(1,i)=index;
end

counts_A=hist(idx_A,8);
idx_AC=zeros(1,8);
for i=1:8
    idx_AC(1,Nearest_C_for_A(1,i))=idx_AC(1,Nearest_C_for_A(1,i))+counts_A(1,i);
end

%-----------------------------
dist_BC=zeros(8,8);
Nearest_C_for_B=zeros(1,8);

for i=1:8
    for j=1:8
        dist_BC(i,j)=dist(Centroid_B(i,:),Centroid_C(j,:)');
    end
end

for i=1:8
    [nearest,index]=min(dist_BC(i,:));
    Nearest_C_for_B(1,i)=index;
end

counts_B=hist(idx_B,8);
idx_BC=zeros(1,8);
for i=1:8
    idx_BC(1,Nearest_C_for_B(1,i))=idx_BC(1,Nearest_C_for_B(1,i))+counts_B(1,i);
end

%-----------------------------
dist_DC=zeros(8,8);
Nearest_C_for_D=zeros(1,8);

for i=1:8
    for j=1:8
        dist_DC(i,j)=dist(Centroid_D(i,:),Centroid_C(j,:)');
    end
end

for i=1:8
    [nearest,index]=min(dist_DC(i,:));
    Nearest_C_for_D(1,i)=index;
end

counts_D=hist(idx_D,8);
idx_DC=zeros(1,8);
for i=1:8
    idx_DC(1,Nearest_C_for_D(1,i))=idx_DC(1,Nearest_C_for_D(1,i))+counts_D(1,i);
end

counts_C=hist(idx_C,8);

counts_A=counts_A';
counts_B=counts_B';
counts_C=counts_C';
counts_D=counts_D';
idx_AC=idx_AC';
idx_BC=idx_BC';
idx_DC=idx_DC';











