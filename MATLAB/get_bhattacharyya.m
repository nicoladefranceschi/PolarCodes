function [ total_samples, Z, Zstd ] = get_bhattacharyya( fold, n, ch_type, ch_par )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

ch_par_txt = num2str(ch_par,'%0.6f');
filename = [fold '/' 'bh_' num2str(n) '_ch_' num2str(ch_type) '_par_' ch_par_txt '.txt'];

fid = fopen(filename, 'r');

if fid == -1
    error 'no file'
end
total_samples=fscanf(fid, '%ld',1);

N=2^n;
Z=zeros(1,N);
Zvar=zeros(1,N);

for i=1:N
    
    Z(i)=double_fget(fid);
    Zvar(i)=double_fget(fid);
    
end

Z=Z./double(total_samples);
Zstd=sqrt( Zvar./double(total_samples) - Z.^2 );

fclose(fid);

end

