function [ x ] = pc_encode( u )
%PC_ENCODE_C Summary of this function goes here
%   Detailed explanation goes here
u=uint8(u);
x=uint8(zeros(size(u)));
for i=1:size(u,1)
    x(i,:)=mex_pc_encode(u(i,:));
end

end

