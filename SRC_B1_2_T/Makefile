#Zachary Job
#Gsyntax MF

NAME=	wTok

SRC= 	waterfall_tokenize.cu \
		\
		\
		Runlib/fBThreadMgr.cu Runlib/fillBuffer.c \
		\
		\
		libs/gpu_binner/Core/wattoken_kernel.cu \
		\
		\
		libs/cpu_parse/Reader/getHeaders.c libs/cpu_parse/Reader/makeMaps.c libs/cpu_parse/Reader/cfgInit.c \
		libs/cpu_parse/Reader/fillMaps.c libs/cpu_parse/Reader/preCompute.c libs/cpu_parse/Reader/loadPreCompute.c \
		\
		\
		libs/cpu_parse/Parser/parseContent.c libs/cpu_parse/Parser/seekInnerTag.c libs/cpu_parse/Parser/initThreadData.c \
		libs/cpu_parse/Parser/seekOuterTag.c libs/cpu_parse/Parser/nextToken.c libs/cpu_parse/Parser/dataInit.c \
		\
		\
		libs/cpu_parse/Shared/readMap.c libs/cpu_parse/Shared/markDB.c libs/cpu_parse/Shared/readDB.c \
		libs/cpu_parse/Shared/offsetToBit.c libs/cpu_parse/Shared/sumAtBit.c libs/cpu_parse/Shared/markMap.c \
		\
		\
		libs/cpu_parse/UTF/getUTFencode.c libs/cpu_parse/UTF/utfCompress.c \

RM=		rm -f

all: digbata

clean:
	-$(RM) *.o

fclean: clean
	-$(RM) $(NAME).exe
	-$(RM) $(NAME)
	-$(RM) *.stackdump

re: fclean all

digbata:
	nvcc $(SRC) -lpthread -o $(NAME)