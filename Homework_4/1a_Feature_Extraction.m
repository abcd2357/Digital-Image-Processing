% EE569 Homework Assignment #4
% Problem 1(a) Texture Classification --- Feature Extraction
% Name: XU Kangyan
% ID: 4876010998
% Email: kangyanx@usc.edu
% Date: Mar 22, 2020
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% FOR TRAIN FILE

% writeraw(Image_out, 'X.raw');
% file_1 = ['train/blanket',num2str(i),'.raw'];

L5 = [1;4;6;4;1];
E5 = [-1;-2;0;2;1];
S5 = [-1;0;2;0;-1];
W5 = [-1;2;0;-2;1];
R5 = [1;-4;6;-4;1];
image = cell(4,9);

for i=1:9
    file_1 = ['train/blanket',num2str(i),'.raw'];
    eval(['image{1,i}','=','readraw','(','file_1',')',';']);
    file_2 = ['train/brick',num2str(i),'.raw'];
    eval(['image{2,i}','=','readraw','(','file_2',')',';']);
    file_3 = ['train/grass',num2str(i),'.raw'];
    eval(['image{3,i}','=','readraw','(','file_3',')',';']);
    file_4 = ['train/rice',num2str(i),'.raw'];
    eval(['image{4,i}','=','readraw','(','file_4',')',';']);
end

% Subtract image mean (reduce illumination effects) 
for i=1:4
    for j=1:9
        image_mean = sum(image{i,j}(:))/(128*128);
        for m=1:128
            for n=1:128
                image{i,j}(m,n) = image{i,j}(m,n) - image_mean;
            end
        end
        %file = [num2str(i),'-',num2str(j),'.raw'];
        %writeraw(image{i,j},file);
    end
end

Laws_filter = cell(1,25);
Laws_filter{1} = L5*L5';Laws_filter{2} = L5*E5';Laws_filter{3} = L5*S5';Laws_filter{4} = L5*W5';Laws_filter{5} = L5*R5';
Laws_filter{6} = E5*L5';Laws_filter{7} = E5*E5';Laws_filter{8} = E5*S5';Laws_filter{9} = E5*W5';Laws_filter{10} = E5*R5';
Laws_filter{11} = S5*L5';Laws_filter{12} = S5*E5';Laws_filter{13} = S5*S5';Laws_filter{14} = S5*W5';Laws_filter{15} = S5*R5';
Laws_filter{16} = W5*L5';Laws_filter{17} = W5*E5';Laws_filter{18} = W5*S5';Laws_filter{19} = W5*W5';Laws_filter{20} = W5*R5';
Laws_filter{21} = R5*L5';Laws_filter{22} = R5*E5';Laws_filter{23} = R5*S5';Laws_filter{24} = R5*W5';Laws_filter{25} = R5*R5';

% Apply 25 5x5 Laws filters on each texture image to get 25 filtered images
feature_vector=cell(4,9);

for i=1:4
    for j=1:9
        
        % Mirror Reflecting Padding 128+2+2
        temp=zeros(132);
        for m=1:128
            for n=1:128
                temp(m+2,n+2) = image{i,j}(m,n);
            end
        end
        
        for n=3:130   
            temp(n,2) = temp(n,3);
            temp(n,1) = temp(n,4); 
        end
            
        for n=1:130    
            temp(1,n) = temp(4,n);
            temp(2,n) = temp(3,n); 
        end
            
        for n=1:130    
            temp(n,131) = temp(n,130);
            temp(n,132) = temp(n,129); 
        end
            
        for n=1:132   
            temp(131,n) = temp(130,n);
            temp(132,n) = temp(129,n); 
        end
        
        % Get 25 filtered images & 25-D feature vector 
       
        for k=1:25
            
            temp_2=zeros(128);
            
            for m=1:128
                for n=1:128
                    for x=-2:2
                        for y=-2:2
                            temp_2(m,n)=temp_2(m,n)+Laws_filter{1,k}(3+x,3+y)*temp(m+2+x,n+2+y);
                        end
                    end
                end
            end
            
            %file = ['lawsfiltered',num2str(i),'-',num2str(j),'-',num2str(k),'.raw'];
            %writeraw(temp_2,file);
            
            % classification, average the image, using abs
            
            temp_abs = abs(temp_2);
            feature_vector{i,j}(k,1)=sum(temp_abs(:))/(128*128);
        
        end
        
    end
end

% 15-D feature vector L5L5 L5E5/E5L5 E5S5/S5E5 E5E5 L5S5/S5L5 E5W5/W5E5 S5S5 L5W5/W5L5 E5R5/R5E5 W5W5 L5R5/R5L5 S5W5/W5S5 R5R5 W5R5/R5W5 S5R5/R5S5 
feature_vector_15D=cell(4,9);


for i=1:4
    for j=1:9
        feature_vector_15D{i,j}(1,1)=feature_vector{i,j}(1,1); % L5L5
        feature_vector_15D{i,j}(2,1)=(feature_vector{i,j}(2,1)+feature_vector{i,j}(6,1))/2; % L5E5/E5L5
        feature_vector_15D{i,j}(3,1)=(feature_vector{i,j}(8,1)+feature_vector{i,j}(12,1))/2; % E5S5/S5E5
        feature_vector_15D{i,j}(4,1)=feature_vector{i,j}(7,1); % E5E5
        feature_vector_15D{i,j}(5,1)=(feature_vector{i,j}(3,1)+feature_vector{i,j}(11,1))/2; % L5S5/S5L5
        feature_vector_15D{i,j}(6,1)=(feature_vector{i,j}(9,1)+feature_vector{i,j}(17,1))/2; % E5W5/W5E5
        feature_vector_15D{i,j}(7,1)=feature_vector{i,j}(13,1); % S5S5
        feature_vector_15D{i,j}(8,1)=(feature_vector{i,j}(4,1)+feature_vector{i,j}(16,1))/2; % L5W5/W5L5
        feature_vector_15D{i,j}(9,1)=(feature_vector{i,j}(10,1)+feature_vector{i,j}(22,1))/2; % E5R5/R5E5
        feature_vector_15D{i,j}(10,1)=feature_vector{i,j}(19,1); % W5W5
        feature_vector_15D{i,j}(11,1)=(feature_vector{i,j}(5,1)+feature_vector{i,j}(21,1))/2; % L5R5/R5L5
        feature_vector_15D{i,j}(12,1)=(feature_vector{i,j}(14,1)+feature_vector{i,j}(18,1))/2; % S5W5/W5S5
        feature_vector_15D{i,j}(13,1)=feature_vector{i,j}(25,1); % R5R5
        feature_vector_15D{i,j}(14,1)=(feature_vector{i,j}(20,1)+feature_vector{i,j}(24,1))/2; % W5R5/R5W5
        feature_vector_15D{i,j}(15,1)=(feature_vector{i,j}(15,1)+feature_vector{i,j}(23,1))/2; % S5R5/R5S5
    end
end

% Evaluate the discriminant power of those features
% Ratio of interclass Var & intraclass Var
discriminant_power=zeros(15,4);
% intraclass Var
vector_temp1=zeros(4,9);
% interclass Var
vector_temp2=zeros(1,36);

for dim=1:15

    % inter-class variance
    for i=1:4
        for j=1:9
            vector_temp2(1,9*i+j-9)=feature_vector_15D{i,j}(dim,1);
        end
    end

    % intra-class variance
    for i=1:4
        for j=1:9
            vector_temp1(i,j)=feature_vector_15D{i,j}(dim,1);
        end
        discriminant_power(dim,i)=var(vector_temp2(1,:))/var(vector_temp1(i,:));
    end
    
end

% Feature Reduction - PCA
X=zeros(36,15);
for i=1:4
    for j=1:9
        X(9*i+j-9,:)=feature_vector_15D{i,j}(:,1);
    end
end

Y=zeros(36,15);
for j=1:15
    mean_Xj=sum(X(:,j))/36;
    Y(:,j)=X(:,j)-mean_Xj;
end

% Singular Value Decomposition

s=svds(Y); % e-value
[U,S,V]=svds(Y);
Vr=V(:,1:3); % e-vector
Yr=Y*Vr; % X or Y same effect

% Plot reduced 3D feature vector
plot_x=Yr(:,1);
plot_y=Yr(:,2);
plot_z=Yr(:,3);
size=20;

c=zeros(36,3);
c(1:9,:)=repmat([1 0 0],9,1);
c(10:18,:)=repmat([0 1 0],9,1);
c(19:27,:)=repmat([0 0 1],9,1);
c(28:36,:)=repmat([1 0 1],9,1);

scatter3(plot_x,plot_y,plot_z,size,c,'filled');
xlabel('X');ylabel('Y');zlabel('Z');
%view([0,0]); % XZ
%view([0,90]); % XY
%view([90,0]); % YZ












