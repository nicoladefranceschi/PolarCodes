function [ total_samples, BER, BERstddev, frame_errors ] = get_error_rate( fold, n, ch_type, ch_par, K)
%GET_ERROR_RATE Summary of this function goes here
%   Detailed explanation goes here
%
%   GET_ERROR_RATE(dir, n, ch_type, ch_par, K) 
%
%   GET_ERROR_RATE(filename)

if nargin == 1
    filename = fold;
else
    ch_par_txt = num2str(ch_par,'%0.6f');
    filename = [fold '/' 'er_' num2str(n) '_ch_' num2str(ch_type) '_par_' ch_par_txt '_k_' num2str(K) '.txt'];
end

fid = fopen(filename, 'r');

if fid == -1
    error 'no file'
end

total_samples=fscanf(fid, '%ld',1);
BER=double_fget(fid);
BER_2=double_fget(fid);
frame_errors=fscanf(fid, '%ld',1);

fclose(fid);


BER = BER / double(total_samples);
BERstddev = sqrt( BER_2 / double(total_samples) - BER^2 );

end

