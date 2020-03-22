function G = readrawNS(filename,X,Y)
%readraw - read RAW format grey scale image
% Usage:	G = readrawNS(filename)

	disp(['	Retrieving Image ' filename ' ...']);

	% Get file ID for file
	fid=fopen(filename,'rb');

	% Check if file exists
	if (fid == -1)
	  	error('can not open input image file press CTRL-C to exit \n');
	  	pause
	end

	% Get all the pixels from the image
	pixel = fread(fid, inf, 'uchar');

	% Close file
	fclose(fid);

	% Construct matrix
	G = zeros(Y,X);
    
    for i=1:Y
        for j=1:X
            G(i,j) = pixel(i*X+j-X);
        end
    end
    
end %function
