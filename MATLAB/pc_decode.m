function [ u_ ] = pc_decode( y, ch_type, ch_par, A)
%PC_DECODE_C Summary of this function goes here
%   Detailed explanation goes here

y=uint8(y);
u_=uint8(zeros(size(y)));
for i=1:size(y,1)
    u_(i,:) = mex_pc_decode(y(i,:), ch_type, ch_par, uint8(A));
end


end

