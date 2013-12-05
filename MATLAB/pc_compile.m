
% compile mex files



mex ../PolarCodes/pc_encode_mex.c ../PolarCodes/pc_encode.c -output 'mex_pc_encode'
mex ../PolarCodes/pc_decode_mex.c ../PolarCodes/channels.c ../PolarCodes/polar_codes.c ../PolarCodes/pc_decode.c ../PolarCodes/pc_decode_internal.c  -output 'mex_pc_decode'




