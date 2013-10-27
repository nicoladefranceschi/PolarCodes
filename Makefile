HEADERS = PolarCodes/bhattacharyya_files.h \
		PolarCodes/channels.h \
		PolarCodes/double_ei.h \
		PolarCodes/pc_construction.h \
		PolarCodes/pc_decode_internal.h \
		PolarCodes/pc_decode.h \
		PolarCodes/pc_encode.h \
		PolarCodes/polar_codes.h 

OBJECTS = PolarCodes/bhattacharyya_files.o \
 	PolarCodes/channels.o \
 	PolarCodes/double_ei.o \
 	PolarCodes/pc_construction.o \
 	PolarCodes/pc_decode_internal.o \
 	PolarCodes/pc_decode.o \
 	PolarCodes/pc_encode.o \
 	PolarCodes/polar_codes.o \
 	PC_pc_construction/main.o

EXECUTABLE = pc_construction

pc_construction: $(OBJECTS)
	gcc $(OBJECTS) -lm -o $@




default: $(EXECUTABLE)

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f $(EXECUTABLE)