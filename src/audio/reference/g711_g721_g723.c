G711_G721_G723/                                                                                     000755  040207  037200  00000000000 05434410645 011766  5                                                                                                    ustar 00ajr                             divfstf                                                                                                                                                                                                                G711_G721_G723/Makefile                                                                             000444  040207  037200  00000001071 05250514235 013417  0                                                                                                    ustar 00ajr                             divfstf                                                                                                                                                                                                                # Makefile for CCITT subroutines and sample programs
#

# Modify the CC definition to invoke your ANSI-C compiler
#
CC = acc


# Rules to build sample programs do not depend on a UNIX library program
#
ALL = encode decode
all: $(ALL)

OBJECTS = g711.o g72x.o g721.o g723_24.o g723_40.o

encode: $(OBJECTS)
	$(CC) -o $@ encode.c $(OBJECTS)

decode: $(OBJECTS)
	$(CC) -o $@ decode.c $(OBJECTS)


# Library rules for UNIX systems
#
LIB = ccitt.a
lib: $(LIB)

$(LIB): $(OBJECTS)
	@rm -f $(LIB)
	ar cr $(LIB) $(OBJECTS)
	@ranlib $(LIB)

clean:
	/bin/rm -f $(LIB) $(ALL) *.o
 Modify the CC definition to invoke your ANSI-C compiler
#
CC = acc


# Rules to build sample programs do not depend on a UNIX library program
#
ALL = encode decode
all: $(ALL)

OBJECTS = g711.o g72x.o g721.o g723_24.o g723_40.o

encode: $(OBJECTS)
	$(CC) -o $@ encode.c $(OBJECTS)

decode: $(OBJECTS)
	$(CC) -o $@ decode.c $(OBJECTS)


# Library rules for UNIX systems
#
LIB = ccitt.a
lib: $(LIB)

$(LIB): $(OBJECTS)
	@rm -f $(LIB)
	ar cr $(LIB) $(OBJECTG711_G721_G723/README                                                                               000444  040207  037200  00000006221 05250514235 012641  0                                                                                                    ustar 00ajr                             divfstf                                                                                                                                                                                                                The files in this directory comprise ANSI-C language reference implementations
of the CCITT (International Telegraph and Telephone Consultative Committee)
G.711, G.721 and G.723 voice compressions.  They have been tested on Sun
SPARCstations and passed 82 out of 84 test vectors published by CCITT
(Dec. 20, 1988) for G.721 and G.723.  [The two remaining test vectors,
which the G.721 decoder implementation for u-law samples did not pass,
may be in error because they are identical to two other vectors for G.723_40.]

This source code is released by Sun Microsystems, Inc. to the public domain.
Please give your acknowledgement in product literature if this code is used
in your product implementation.

Sun Microsystems supports some CCITT audio formats in Solaris 2.0 system
software.  However, Sun's implementations have been optimized for higher
performance on SPARCstations.


The source files for CCITT conversion routines in this directory are:

	g72x.h		header file for g721.c, g723_24.c and g723_40.c
	g711.c		CCITT G.711 u-law and A-law compression
	g72x.c		common denominator of G.721 and G.723 ADPCM codes
	g721.c		CCITT G.721 32Kbps ADPCM coder (with g72x.c)
	g723_24.c	CCITT G.723 24Kbps ADPCM coder (with g72x.c)
	g723_40.c	CCITT G.723 40Kbps ADPCM coder (with g72x.c)


Simple conversions between u-law, A-law, and 16-bit linear PCM are invoked
as follows:

	unsigned char		ucode, acode;
	short			pcm_val;

	ucode = linear2ulaw(pcm_val);
	ucode = alaw2ulaw(acode);

	acode = linear2alaw(pcm_val);
	acode = ulaw2alaw(ucode);

	pcm_val = ulaw2linear(ucode);
	pcm_val = alaw2linear(acode);


The other CCITT compression routines are invoked as follows:

	#include "g72x.h"

	struct g72x_state	state;
	int			sample, code;

	g72x_init_state(&state);
	code = {g721,g723_24,g723_40}_encoder(sample, coding, &state);
	sample = {g721,g723_24,g723_40}_decoder(code, coding, &state);

where
	coding = AUDIO_ENCODING_ULAW	for 8-bit u-law samples
		 AUDIO_ENCODING_ALAW	for 8-bit A-law samples
		 AUDIO_ENCODING_LINEAR	for 16-bit linear PCM samples



This directory also includes the following sample programs:

	encode.c	CCITT ADPCM encoder
	decode.c	CCITT ADPCM decoder
	Makefile	makefile for the sample programs


The sample programs contain examples of how to call the various compression
routines and pack/unpack the bits.  The sample programs read byte streams from
stdin and write to stdout.  The input/output data is raw data (no file header
or other identifying information is embedded).  The sample programs are
invoked as follows:

	encode [-3|4|5] [-a|u|l] <infile >outfile
	decode [-3|4|5] [-a|u|l] <infile >outfile
where:
	-3	encode to (decode from) G.723 24kbps (3-bit) data
	-4	encode to (decode from) G.721 32kbps (4-bit) data [the default]
	-5	encode to (decode from) G.723 40kbps (5-bit) data
	-a	encode from (decode to) A-law data
	-u	encode from (decode to) u-law data [the default]
	-l	encode from (decode to) 16-bit linear data

Examples:
	# Read 16-bit linear and output G.721
	encode -4 -l <pcmfile >g721file

	# Read 40Kbps G.723 and output A-law
	decode -5 -a <g723file >alawfile

	# Compress and then decompress u-law data using 24Kbps G.723
	encode -3 <ulawin | deoced -3 >ulawout

e to (decode from) G.721 32kbps (4-bit) data [the default]
	-5	encode to (decode from) G.723 40kbps (5-bit) data
	-a	encode from (decode to) A-law data
	-u	encode from (decode to) u-law data [the default]
	-l	encode from (decode to) 16-bit linear data

Examples:
	# Read 16-bit linear and output G.721
	encode -4 -l <pcmfile >g721file

	# Read 40Kbps G.723 and outputG711_G721_G723/decode.c                                                                             000444  040207  037200  00000004737 05250514235 013362  0                                                                                                    ustar 00ajr                             divfstf                                                                                                                                                                                                                /*
 * decode.c
 *
 * CCITT ADPCM decoder
 *
 * Usage : decode [-3|4|5] [-a|u|l] < infile > outfile
 */
#include <stdio.h>
#include "g72x.h"


/*
 * Unpack input codes and pass them back as bytes.
 * Returns 1 if there is residual input, returns -1 if eof, else returns 0.
 */
int
unpack_input(
	unsigned char		*code,
	int			bits)
{
	static unsigned int	in_buffer = 0;
	static int		in_bits = 0;
	unsigned char		in_byte;

	if (in_bits < bits) {
		if (fread(&in_byte, sizeof (char), 1, stdin) != 1) {
			*code = 0;
			return (-1);
		}
		in_buffer |= (in_byte << in_bits);
		in_bits += 8;
	}
	*code = in_buffer & ((1 << bits) - 1);
	in_buffer >>= bits;
	in_bits -= bits;
	return (in_bits > 0);
}


main(
	int			argc,
	char			**argv)
{
	short			sample;
	unsigned char		code;
	int			n;
	struct g72x_state	state;
	int			out_coding;
	int			out_size;
	int			(*dec_routine)();
	int			dec_bits;

	g72x_init_state(&state);
	out_coding = AUDIO_ENCODING_ULAW;
	out_size = sizeof (char);
	dec_routine = g721_decoder;
	dec_bits = 4;

	/* Process encoding argument, if any */
	while ((argc > 1) && (argv[1][0] == '-')) {
		switch (argv[1][1]) {
		case '3':
			dec_routine = g723_24_decoder;
			dec_bits = 3;
			break;
		case '4':
			dec_routine = g721_decoder;
			dec_bits = 4;
			break;
		case '5':
			dec_routine = g723_40_decoder;
			dec_bits = 5;
			break;
		case 'u':
			out_coding = AUDIO_ENCODING_ULAW;
			out_size = sizeof (char);
			break;
		case 'a':
			out_coding = AUDIO_ENCODING_ALAW;
			out_size = sizeof (char);
			break;
		case 'l':
			out_coding = AUDIO_ENCODING_LINEAR;
			out_size = sizeof (short);
			break;
		default:
fprintf(stderr, "CCITT ADPCM Decoder -- usage:\n");
fprintf(stderr, "\tdecode [-3|4|5] [-a|u|l] < infile > outfile\n");
fprintf(stderr, "where:\n");
fprintf(stderr, "\t-3\tProcess G.723 24kbps (3-bit) input data\n");
fprintf(stderr, "\t-4\tProcess G.721 32kbps (4-bit) input data [default]\n");
fprintf(stderr, "\t-5\tProcess G.723 40kbps (5-bit) input data\n");
fprintf(stderr, "\t-a\tGenerate 8-bit A-law data\n");
fprintf(stderr, "\t-u\tGenerate 8-bit u-law data [default]\n");
fprintf(stderr, "\t-l\tGenerate 16-bit linear PCM data\n");
			exit(1);
		}
		argc--;
		argv++;
	}

	/* Read and unpack input codes and process them */
	while (unpack_input(&code, dec_bits) >= 0) {
		sample = (*dec_routine)(code, out_coding, &state);
		if (out_size == 2) {
			fwrite(&sample, out_size, 1, stdout);
		} else {
			code = (unsigned char)sample;
			fwrite(&code, out_size, 1, stdout);
		}
	}
	fclose(stdout);
}
e 8-bit A-law data\n");
fprintf(sG711_G721_G723/encode.c                                                                             000444  040207  037200  00000005273 05250514235 013370  0                                                                                                    ustar 00ajr                             divfstf                                                                                                                                                                                                                /*
 * encode.c
 *
 * CCITT ADPCM encoder
 *
 * Usage : encode [-3|4|5] [-a|u|l] < infile > outfile
 */
#include <stdio.h>
#include "g72x.h"


/*
 * Pack output codes into bytes and write them to stdout.
 * Returns 1 if there is residual output, else returns 0.
 */
int
pack_output(
	unsigned		code,
	int			bits)
{
	static unsigned int	out_buffer = 0;
	static int		out_bits = 0;
	unsigned char		out_byte;

	out_buffer |= (code << out_bits);
	out_bits += bits;
	if (out_bits >= 8) {
		out_byte = out_buffer & 0xff;
		out_bits -= 8;
		out_buffer >>= 8;
		fwrite(&out_byte, sizeof (char), 1, stdout);
	}
	return (out_bits > 0);
}


main(
	int			argc,
	char			**argv)
{
	struct g72x_state	state;
	unsigned char		sample_char;
	short			sample_short;
	unsigned char		code;
	int			resid;
	int			in_coding;
	int			in_size;
	unsigned		*in_buf;
	int			(*enc_routine)();
	int			enc_bits;

	g72x_init_state(&state);

	/* Set defaults to u-law input, G.721 output */
	in_coding = AUDIO_ENCODING_ULAW;
	in_size = sizeof (char);
	in_buf = (unsigned *)&sample_char;
	enc_routine = g721_encoder;
	enc_bits = 4;

	/* Process encoding argument, if any */
	while ((argc > 1) && (argv[1][0] == '-')) {
		switch (argv[1][1]) {
		case '3':
			enc_routine = g723_24_encoder;
			enc_bits = 3;
			break;
		case '4':
			enc_routine = g721_encoder;
			enc_bits = 4;
			break;
		case '5':
			enc_routine = g723_40_encoder;
			enc_bits = 5;
			break;
		case 'u':
			in_coding = AUDIO_ENCODING_ULAW;
			in_size = sizeof (char);
			in_buf = (unsigned *)&sample_char;
			break;
		case 'a':
			in_coding = AUDIO_ENCODING_ALAW;
			in_size = sizeof (char);
			in_buf = (unsigned *)&sample_char;
			break;
		case 'l':
			in_coding = AUDIO_ENCODING_LINEAR;
			in_size = sizeof (short);
			in_buf = (unsigned *)&sample_short;
			break;
		default:
fprintf(stderr, "CCITT ADPCM Encoder -- usage:\n");
fprintf(stderr, "\tencode [-3|4|5] [-a|u|l] < infile > outfile\n");
fprintf(stderr, "where:\n");
fprintf(stderr, "\t-3\tGenerate G.723 24kbps (3-bit) data\n");
fprintf(stderr, "\t-4\tGenerate G.721 32kbps (4-bit) data [default]\n");
fprintf(stderr, "\t-5\tGenerate G.723 40kbps (5-bit) data\n");
fprintf(stderr, "\t-a\tProcess 8-bit A-law input data\n");
fprintf(stderr, "\t-u\tProcess 8-bit u-law input data [default]\n");
fprintf(stderr, "\t-l\tProcess 16-bit linear PCM input data\n");
			exit(1);
		}
		argc--;
		argv++;
	}

	/* Read input file and process */
	while (fread(in_buf, in_size, 1, stdin) == 1) {
		code = (*enc_routine)(in_size == 2 ? sample_short : sample_char,
		    in_coding, &state);
		resid = pack_output(code, enc_bits);
	}

	/* Write zero codes until all residual codes are written out */
	while (resid) {
		resid = pack_output(0, enc_bits);
	}
	fclose(stdout);
}
tProcess 8-bit u-law input data [default]\n");
fprintf(stderr, "\t-l\tProcess 16-bit linear PCM input data\n");
			exit(1);
		}
		argc--;
		argv++;
	}

	/* Read input file and process */
	while (fread(in_buf, in_size, 1, stdin) == 1) {
		code = (*enc_routine)(in_size == 2 ? sample_short : sample_char,
		    in_coding, &statG711_G721_G723/g711.c                                                                               000444  040207  037200  00000016541 05250514235 012612  0                                                                                                    ustar 00ajr                             divfstf                                                                                                                                                                                                                /*
 * This source code is a product of Sun Microsystems, Inc. and is provided
 * for unrestricted use.  Users may copy or modify this source code without
 * charge.
 *
 * SUN SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING
 * THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun source code is provided with no support and without any obligation on
 * the part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY THIS SOFTWARE
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */

/*
 * g711.c
 *
 * u-law, A-law and linear PCM conversions.
 */
#define	SIGN_BIT	(0x80)		/* Sign bit for a A-law byte. */
#define	QUANT_MASK	(0xf)		/* Quantization field mask. */
#define	NSEGS		(8)		/* Number of A-law segments. */
#define	SEG_SHIFT	(4)		/* Left shift for segment number. */
#define	SEG_MASK	(0x70)		/* Segment field mask. */

static short seg_end[8] = {0xFF, 0x1FF, 0x3FF, 0x7FF,
			    0xFFF, 0x1FFF, 0x3FFF, 0x7FFF};

/* copy from CCITT G.711 specifications */
unsigned char _u2a[128] = {			/* u- to A-law conversions */
	1,	1,	2,	2,	3,	3,	4,	4,
	5,	5,	6,	6,	7,	7,	8,	8,
	9,	10,	11,	12,	13,	14,	15,	16,
	17,	18,	19,	20,	21,	22,	23,	24,
	25,	27,	29,	31,	33,	34,	35,	36,
	37,	38,	39,	40,	41,	42,	43,	44,
	46,	48,	49,	50,	51,	52,	53,	54,
	55,	56,	57,	58,	59,	60,	61,	62,
	64,	65,	66,	67,	68,	69,	70,	71,
	72,	73,	74,	75,	76,	77,	78,	79,
	81,	82,	83,	84,	85,	86,	87,	88,
	89,	90,	91,	92,	93,	94,	95,	96,
	97,	98,	99,	100,	101,	102,	103,	104,
	105,	106,	107,	108,	109,	110,	111,	112,
	113,	114,	115,	116,	117,	118,	119,	120,
	121,	122,	123,	124,	125,	126,	127,	128};

unsigned char _a2u[128] = {			/* A- to u-law conversions */
	1,	3,	5,	7,	9,	11,	13,	15,
	16,	17,	18,	19,	20,	21,	22,	23,
	24,	25,	26,	27,	28,	29,	30,	31,
	32,	32,	33,	33,	34,	34,	35,	35,
	36,	37,	38,	39,	40,	41,	42,	43,
	44,	45,	46,	47,	48,	48,	49,	49,
	50,	51,	52,	53,	54,	55,	56,	57,
	58,	59,	60,	61,	62,	63,	64,	64,
	65,	66,	67,	68,	69,	70,	71,	72,
	73,	74,	75,	76,	77,	78,	79,	79,
	80,	81,	82,	83,	84,	85,	86,	87,
	88,	89,	90,	91,	92,	93,	94,	95,
	96,	97,	98,	99,	100,	101,	102,	103,
	104,	105,	106,	107,	108,	109,	110,	111,
	112,	113,	114,	115,	116,	117,	118,	119,
	120,	121,	122,	123,	124,	125,	126,	127};

static int
search(
	int		val,
	short		*table,
	int		size)
{
	int		i;

	for (i = 0; i < size; i++) {
		if (val <= *table++)
			return (i);
	}
	return (size);
}

/*
 * linear2alaw() - Convert a 16-bit linear PCM value to 8-bit A-law
 *
 * linear2alaw() accepts an 16-bit integer and encodes it as A-law data.
 *
 *		Linear Input Code	Compressed Code
 *	------------------------	---------------
 *	0000000wxyza			000wxyz
 *	0000001wxyza			001wxyz
 *	000001wxyzab			010wxyz
 *	00001wxyzabc			011wxyz
 *	0001wxyzabcd			100wxyz
 *	001wxyzabcde			101wxyz
 *	01wxyzabcdef			110wxyz
 *	1wxyzabcdefg			111wxyz
 *
 * For further information see John C. Bellamy's Digital Telephony, 1982,
 * John Wiley & Sons, pps 98-111 and 472-476.
 */
unsigned char
linear2alaw(
	int		pcm_val)	/* 2's complement (16-bit range) */
{
	int		mask;
	int		seg;
	unsigned char	aval;

	if (pcm_val >= 0) {
		mask = 0xD5;		/* sign (7th) bit = 1 */
	} else {
		mask = 0x55;		/* sign bit = 0 */
		pcm_val = -pcm_val - 8;
	}

	/* Convert the scaled magnitude to segment number. */
	seg = search(pcm_val, seg_end, 8);

	/* Combine the sign, segment, and quantization bits. */

	if (seg >= 8)		/* out of range, return maximum value. */
		return (0x7F ^ mask);
	else {
		aval = seg << SEG_SHIFT;
		if (seg < 2)
			aval |= (pcm_val >> 4) & QUANT_MASK;
		else
			aval |= (pcm_val >> (seg + 3)) & QUANT_MASK;
		return (aval ^ mask);
	}
}

/*
 * alaw2linear() - Convert an A-law value to 16-bit linear PCM
 *
 */
int
alaw2linear(
	unsigned char	a_val)
{
	int		t;
	int		seg;

	a_val ^= 0x55;

	t = (a_val & QUANT_MASK) << 4;
	seg = ((unsigned)a_val & SEG_MASK) >> SEG_SHIFT;
	switch (seg) {
	case 0:
		t += 8;
		break;
	case 1:
		t += 0x108;
		break;
	default:
		t += 0x108;
		t <<= seg - 1;
	}
	return ((a_val & SIGN_BIT) ? t : -t);
}

#define	BIAS		(0x84)		/* Bias for linear code. */

/*
 * linear2ulaw() - Convert a linear PCM value to u-law
 *
 * In order to simplify the encoding process, the original linear magnitude
 * is biased by adding 33 which shifts the encoding range from (0 - 8158) to
 * (33 - 8191). The result can be seen in the following encoding table:
 *
 *	Biased Linear Input Code	Compressed Code
 *	------------------------	---------------
 *	00000001wxyza			000wxyz
 *	0000001wxyzab			001wxyz
 *	000001wxyzabc			010wxyz
 *	00001wxyzabcd			011wxyz
 *	0001wxyzabcde			100wxyz
 *	001wxyzabcdef			101wxyz
 *	01wxyzabcdefg			110wxyz
 *	1wxyzabcdefgh			111wxyz
 *
 * Each biased linear code has a leading 1 which identifies the segment
 * number. The value of the segment number is equal to 7 minus the number
 * of leading 0's. The quantization interval is directly available as the
 * four bits wxyz.  * The trailing bits (a - h) are ignored.
 *
 * Ordinarily the complement of the resulting code word is used for
 * transmission, and so the code word is complemented before it is returned.
 *
 * For further information see John C. Bellamy's Digital Telephony, 1982,
 * John Wiley & Sons, pps 98-111 and 472-476.
 */
unsigned char
linear2ulaw(
	int		pcm_val)	/* 2's complement (16-bit range) */
{
	int		mask;
	int		seg;
	unsigned char	uval;

	/* Get the sign and the magnitude of the value. */
	if (pcm_val < 0) {
		pcm_val = BIAS - pcm_val;
		mask = 0x7F;
	} else {
		pcm_val += BIAS;
		mask = 0xFF;
	}

	/* Convert the scaled magnitude to segment number. */
	seg = search(pcm_val, seg_end, 8);

	/*
	 * Combine the sign, segment, quantization bits;
	 * and complement the code word.
	 */
	if (seg >= 8)		/* out of range, return maximum value. */
		return (0x7F ^ mask);
	else {
		uval = (seg << 4) | ((pcm_val >> (seg + 3)) & 0xF);
		return (uval ^ mask);
	}

}

/*
 * ulaw2linear() - Convert a u-law value to 16-bit linear PCM
 *
 * First, a biased linear code is derived from the code word. An unbiased
 * output can then be obtained by subtracting 33 from the biased code.
 *
 * Note that this function expects to be passed the complement of the
 * original code word. This is in keeping with ISDN conventions.
 */
int
ulaw2linear(
	unsigned char	u_val)
{
	int		t;

	/* Complement to obtain normal u-law value. */
	u_val = ~u_val;

	/*
	 * Extract and bias the quantization bits. Then
	 * shift up by the segment number and subtract out the bias.
	 */
	t = ((u_val & QUANT_MASK) << 3) + BIAS;
	t <<= ((unsigned)u_val & SEG_MASK) >> SEG_SHIFT;

	return ((u_val & SIGN_BIT) ? (BIAS - t) : (t - BIAS));
}

/* A-law to u-law conversion */
unsigned char
alaw2ulaw(
	unsigned char	aval)
{
	aval &= 0xff;
	return ((aval & 0x80) ? (0xFF ^ _a2u[aval ^ 0xD5]) :
	    (0x7F ^ _a2u[aval ^ 0x55]));
}

/* u-law to A-law conversion */
unsigned char
ulaw2alaw(
	unsigned char	uval)
{
	uval &= 0xff;
	return ((uval & 0x80) ? (0xD5 ^ (_u2a[0xFF ^ uval] - 1)) :
	    (0x55 ^ (_u2a[0x7F ^ uval] - 1)));
}
ASK) << 3) + BIAS;
	t <<= ((unsigned)u_val & SEG_MASK) >> SEG_SHIFT;

	return ((u_val & SIGN_BIT) ? (BIAS - t) : (t - BIAS));
}

/* A-law to u-law conversion *G711_G721_G723/g721.c                                                                               000444  040207  037200  00000012211 05250514235 012601  0                                                                                                    ustar 00ajr                             divfstf                                                                                                                                                                                                                /*
 * This source code is a product of Sun Microsystems, Inc. and is provided
 * for unrestricted use.  Users may copy or modify this source code without
 * charge.
 *
 * SUN SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING
 * THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun source code is provided with no support and without any obligation on
 * the part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY THIS SOFTWARE
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */

/*
 * g721.c
 *
 * Description:
 *
 * g721_encoder(), g721_decoder()
 *
 * These routines comprise an implementation of the CCITT G.721 ADPCM
 * coding algorithm.  Essentially, this implementation is identical to
 * the bit level description except for a few deviations which
 * take advantage of work station attributes, such as hardware 2's
 * complement arithmetic and large memory.  Specifically, certain time
 * consuming operations such as multiplications are replaced
 * with lookup tables and software 2's complement operations are
 * replaced with hardware 2's complement.
 *
 * The deviation from the bit level specification (lookup tables)
 * preserves the bit level performance specifications.
 *
 * As outlined in the G.721 Recommendation, the algorithm is broken
 * down into modules.  Each section of code below is preceded by
 * the name of the module which it is implementing.
 *
 */
#include "g72x.h"

static short qtab_721[7] = {-124, 80, 178, 246, 300, 349, 400};
/*
 * Maps G.721 code word to reconstructed scale factor normalized log
 * magnitude values.
 */
static short	_dqlntab[16] = {-2048, 4, 135, 213, 273, 323, 373, 425,
				425, 373, 323, 273, 213, 135, 4, -2048};

/* Maps G.721 code word to log of scale factor multiplier. */
static short	_witab[16] = {-12, 18, 41, 64, 112, 198, 355, 1122,
				1122, 355, 198, 112, 64, 41, 18, -12};
/*
 * Maps G.721 code words to a set of values whose long and short
 * term averages are computed and then compared to give an indication
 * how stationary (steady state) the signal is.
 */
static short	_fitab[16] = {0, 0, 0, 0x200, 0x200, 0x200, 0x600, 0xE00,
				0xE00, 0x600, 0x200, 0x200, 0x200, 0, 0, 0};

/*
 * g721_encoder()
 *
 * Encodes the input vale of linear PCM, A-law or u-law data sl and returns
 * the resulting code. -1 is returned for unknown input coding value.
 */
int
g721_encoder(
	int		sl,
	int		in_coding,
	struct g72x_state *state_ptr)
{
	short		sezi, se, sez;		/* ACCUM */
	short		d;			/* SUBTA */
	short		sr;			/* ADDB */
	short		y;			/* MIX */
	short		dqsez;			/* ADDC */
	short		dq, i;

	switch (in_coding) {	/* linearize input sample to 14-bit PCM */
	case AUDIO_ENCODING_ALAW:
		sl = alaw2linear(sl) >> 2;
		break;
	case AUDIO_ENCODING_ULAW:
		sl = ulaw2linear(sl) >> 2;
		break;
	case AUDIO_ENCODING_LINEAR:
		sl >>= 2;			/* 14-bit dynamic range */
		break;
	default:
		return (-1);
	}

	sezi = predictor_zero(state_ptr);
	sez = sezi >> 1;
	se = (sezi + predictor_pole(state_ptr)) >> 1;	/* estimated signal */

	d = sl - se;				/* estimation difference */

	/* quantize the prediction difference */
	y = step_size(state_ptr);		/* quantizer step size */
	i = quantize(d, y, qtab_721, 7);	/* i = ADPCM code */

	dq = reconstruct(i & 8, _dqlntab[i], y);	/* quantized est diff */

	sr = (dq < 0) ? se - (dq & 0x3FFF) : se + dq;	/* reconst. signal */

	dqsez = sr + sez - se;			/* pole prediction diff. */

	update(4, y, _witab[i] << 5, _fitab[i], dq, sr, dqsez, state_ptr);

	return (i);
}

/*
 * g721_decoder()
 *
 * Description:
 *
 * Decodes a 4-bit code of G.721 encoded data of i and
 * returns the resulting linear PCM, A-law or u-law value.
 * return -1 for unknown out_coding value.
 */
int
g721_decoder(
	int		i,
	int		out_coding,
	struct g72x_state *state_ptr)
{
	short		sezi, sei, sez, se;	/* ACCUM */
	short		y;			/* MIX */
	short		sr;			/* ADDB */
	short		dq;
	short		dqsez;

	i &= 0x0f;			/* mask to get proper bits */
	sezi = predictor_zero(state_ptr);
	sez = sezi >> 1;
	sei = sezi + predictor_pole(state_ptr);
	se = sei >> 1;			/* se = estimated signal */

	y = step_size(state_ptr);	/* dynamic quantizer step size */

	dq = reconstruct(i & 0x08, _dqlntab[i], y); /* quantized diff. */

	sr = (dq < 0) ? (se - (dq & 0x3FFF)) : se + dq;	/* reconst. signal */

	dqsez = sr - se + sez;			/* pole prediction diff. */

	update(4, y, _witab[i] << 5, _fitab[i], dq, sr, dqsez, state_ptr);

	switch (out_coding) {
	case AUDIO_ENCODING_ALAW:
		return (tandem_adjust_alaw(sr, se, y, i, 8, qtab_721));
	case AUDIO_ENCODING_ULAW:
		return (tandem_adjust_ulaw(sr, se, y, i, 8, qtab_721));
	case AUDIO_ENCODING_LINEAR:
		return (sr << 2);	/* sr was 14-bit dynamic range */
	default:
		return (-1);
	}
}
diff. */

	sr = (dq < 0) ? (se - (dq & 0x3FFF)) : se + dq;	/* reconst. signal */

	dqsez = sr - se + sez;			/* pole prediction diff. */

	update(4, y, _witab[i] << 5, _fitab[i], dq, sr, dqsez, state_ptr);

	switch (out_coding) {
	case AUDIO_ENCODING_ALAW:
		return (tandem_adjust_alaw(sr, se, y, i, 8, qtab_721));
	case AUDIO_ENCODING_ULAW:
		return (tandem_adjust_ulaw(sr, sG711_G721_G723/g723_24.c                                                                            000444  040207  037200  00000011035 05250514235 013113  0                                                                                                    ustar 00ajr                             divfstf                                                                                                                                                                                                                /*
 * This source code is a product of Sun Microsystems, Inc. and is provided
 * for unrestricted use.  Users may copy or modify this source code without
 * charge.
 *
 * SUN SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING
 * THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun source code is provided with no support and without any obligation on
 * the part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY THIS SOFTWARE
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */

/*
 * g723_24.c
 *
 * Description:
 *
 * g723_24_encoder(), g723_24_decoder()
 *
 * These routines comprise an implementation of the CCITT G.723 24 Kbps
 * ADPCM coding algorithm.  Essentially, this implementation is identical to
 * the bit level description except for a few deviations which take advantage
 * of workstation attributes, such as hardware 2's complement arithmetic.
 *
 */
#include "g72x.h"

/*
 * Maps G.723_24 code word to reconstructed scale factor normalized log
 * magnitude values.
 */
static short	_dqlntab[8] = {-2048, 135, 273, 373, 373, 273, 135, -2048};

/* Maps G.723_24 code word to log of scale factor multiplier. */
static short	_witab[8] = {-128, 960, 4384, 18624, 18624, 4384, 960, -128};

/*
 * Maps G.723_24 code words to a set of values whose long and short
 * term averages are computed and then compared to give an indication
 * how stationary (steady state) the signal is.
 */
static short	_fitab[8] = {0, 0x200, 0x400, 0xE00, 0xE00, 0x400, 0x200, 0};

static short qtab_723_24[3] = {8, 218, 331};

/*
 * g723_24_encoder()
 *
 * Encodes a linear PCM, A-law or u-law input sample and returns its 3-bit code.
 * Returns -1 if invalid input coding value.
 */
int
g723_24_encoder(
	int		sl,
	int		in_coding,
	struct g72x_state *state_ptr)
{
	short		sei, sezi, se, sez;	/* ACCUM */
	short		d;			/* SUBTA */
	short		y;			/* MIX */
	short		sr;			/* ADDB */
	short		dqsez;			/* ADDC */
	short		dq, i;

	switch (in_coding) {	/* linearize input sample to 14-bit PCM */
	case AUDIO_ENCODING_ALAW:
		sl = alaw2linear(sl) >> 2;
		break;
	case AUDIO_ENCODING_ULAW:
		sl = ulaw2linear(sl) >> 2;
		break;
	case AUDIO_ENCODING_LINEAR:
		sl >>= 2;		/* sl of 14-bit dynamic range */
		break;
	default:
		return (-1);
	}

	sezi = predictor_zero(state_ptr);
	sez = sezi >> 1;
	sei = sezi + predictor_pole(state_ptr);
	se = sei >> 1;			/* se = estimated signal */

	d = sl - se;			/* d = estimation diff. */

	/* quantize prediction difference d */
	y = step_size(state_ptr);	/* quantizer step size */
	i = quantize(d, y, qtab_723_24, 3);	/* i = ADPCM code */
	dq = reconstruct(i & 4, _dqlntab[i], y); /* quantized diff. */

	sr = (dq < 0) ? se - (dq & 0x3FFF) : se + dq; /* reconstructed signal */

	dqsez = sr + sez - se;		/* pole prediction diff. */

	update(3, y, _witab[i], _fitab[i], dq, sr, dqsez, state_ptr);

	return (i);
}

/*
 * g723_24_decoder()
 *
 * Decodes a 3-bit CCITT G.723_24 ADPCM code and returns
 * the resulting 16-bit linear PCM, A-law or u-law sample value.
 * -1 is returned if the output coding is unknown.
 */
int
g723_24_decoder(
	int		i,
	int		out_coding,
	struct g72x_state *state_ptr)
{
	short		sezi, sei, sez, se;	/* ACCUM */
	short		y;			/* MIX */
	short		sr;			/* ADDB */
	short		dq;
	short		dqsez;

	i &= 0x07;			/* mask to get proper bits */
	sezi = predictor_zero(state_ptr);
	sez = sezi >> 1;
	sei = sezi + predictor_pole(state_ptr);
	se = sei >> 1;			/* se = estimated signal */

	y = step_size(state_ptr);	/* adaptive quantizer step size */
	dq = reconstruct(i & 0x04, _dqlntab[i], y); /* unquantize pred diff */

	sr = (dq < 0) ? (se - (dq & 0x3FFF)) : (se + dq); /* reconst. signal */

	dqsez = sr - se + sez;			/* pole prediction diff. */

	update(3, y, _witab[i], _fitab[i], dq, sr, dqsez, state_ptr);

	switch (out_coding) {
	case AUDIO_ENCODING_ALAW:
		return (tandem_adjust_alaw(sr, se, y, i, 4, qtab_723_24));
	case AUDIO_ENCODING_ULAW:
		return (tandem_adjust_ulaw(sr, se, y, i, 4, qtab_723_24));
	case AUDIO_ENCODING_LINEAR:
		return (sr << 2);	/* sr was of 14-bit dynamic range */
	default:
		return (-1);
	}
}
*/

	sr = (dq < 0) ? (se - (dq & 0x3FFF)) : (se + dq); /* reconst. signal */

	dqsez = sr - se + sez;			/* pole prediction diff. */

	update(3, y, _witab[i], _fitab[i], dq, sr, dqsez, state_ptr);

	switch (out_coding) {
	case AUDIO_ENCODING_ALAW:
		return (tandem_adjust_alaw(sr, se, y, i, 4, qtab_723_24));
	case AUDIO_ENCODING_ULAW:
		return (tandem_adjust_ulaw(sr, se, y, i, 4, qtab_723_24));
	case AUDIO_ENCODING_LINEAR:
		return (sr << 2);	/* sr was of 14-bit dynamic range */
	G711_G721_G723/g723_40.c                                                                            000444  040207  037200  00000012631 05250514235 013114  0                                                                                                    ustar 00ajr                             divfstf                                                                                                                                                                                                                /*
 * This source code is a product of Sun Microsystems, Inc. and is provided
 * for unrestricted use.  Users may copy or modify this source code without
 * charge.
 *
 * SUN SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING
 * THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun source code is provided with no support and without any obligation on
 * the part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY THIS SOFTWARE
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */

/*
 * g723_40.c
 *
 * Description:
 *
 * g723_40_encoder(), g723_40_decoder()
 *
 * These routines comprise an implementation of the CCITT G.723 40Kbps
 * ADPCM coding algorithm.  Essentially, this implementation is identical to
 * the bit level description except for a few deviations which
 * take advantage of workstation attributes, such as hardware 2's
 * complement arithmetic.
 *
 * The deviation from the bit level specification (lookup tables),
 * preserves the bit level performance specifications.
 *
 * As outlined in the G.723 Recommendation, the algorithm is broken
 * down into modules.  Each section of code below is preceded by
 * the name of the module which it is implementing.
 *
 */
#include "g72x.h"

/*
 * Maps G.723_40 code word to ructeconstructed scale factor normalized log
 * magnitude values.
 */
static short	_dqlntab[32] = {-2048, -66, 28, 104, 169, 224, 274, 318,
				358, 395, 429, 459, 488, 514, 539, 566,
				566, 539, 514, 488, 459, 429, 395, 358,
				318, 274, 224, 169, 104, 28, -66, -2048};

/* Maps G.723_40 code word to log of scale factor multiplier. */
static short	_witab[32] = {448, 448, 768, 1248, 1280, 1312, 1856, 3200,
			4512, 5728, 7008, 8960, 11456, 14080, 16928, 22272,
			22272, 16928, 14080, 11456, 8960, 7008, 5728, 4512,
			3200, 1856, 1312, 1280, 1248, 768, 448, 448};

/*
 * Maps G.723_40 code words to a set of values whose long and short
 * term averages are computed and then compared to give an indication
 * how stationary (steady state) the signal is.
 */
static short	_fitab[32] = {0, 0, 0, 0, 0, 0x200, 0x200, 0x200,
			0x200, 0x200, 0x400, 0x600, 0x800, 0xA00, 0xC00, 0xC00,
			0xC00, 0xC00, 0xA00, 0x800, 0x600, 0x400, 0x200, 0x200,
			0x200, 0x200, 0x200, 0, 0, 0, 0, 0};

static short qtab_723_40[15] = {-122, -16, 68, 139, 198, 250, 298, 339,
				378, 413, 445, 475, 502, 528, 553};

/*
 * g723_40_encoder()
 *
 * Encodes a 16-bit linear PCM, A-law or u-law input sample and retuens
 * the resulting 5-bit CCITT G.723 40Kbps code.
 * Returns -1 if the input coding value is invalid.
 */
int
g723_40_encoder(
	int		sl,
	int		in_coding,
	struct g72x_state *state_ptr)
{
	short		sei, sezi, se, sez;	/* ACCUM */
	short		d;			/* SUBTA */
	short		y;			/* MIX */
	short		sr;			/* ADDB */
	short		dqsez;			/* ADDC */
	short		dq, i;

	switch (in_coding) {	/* linearize input sample to 14-bit PCM */
	case AUDIO_ENCODING_ALAW:
		sl = alaw2linear(sl) >> 2;
		break;
	case AUDIO_ENCODING_ULAW:
		sl = ulaw2linear(sl) >> 2;
		break;
	case AUDIO_ENCODING_LINEAR:
		sl >>= 2;		/* sl of 14-bit dynamic range */
		break;
	default:
		return (-1);
	}

	sezi = predictor_zero(state_ptr);
	sez = sezi >> 1;
	sei = sezi + predictor_pole(state_ptr);
	se = sei >> 1;			/* se = estimated signal */

	d = sl - se;			/* d = estimation difference */

	/* quantize prediction difference */
	y = step_size(state_ptr);	/* adaptive quantizer step size */
	i = quantize(d, y, qtab_723_40, 15);	/* i = ADPCM code */

	dq = reconstruct(i & 0x10, _dqlntab[i], y);	/* quantized diff */

	sr = (dq < 0) ? se - (dq & 0x7FFF) : se + dq; /* reconstructed signal */

	dqsez = sr + sez - se;		/* dqsez = pole prediction diff. */

	update(5, y, _witab[i], _fitab[i], dq, sr, dqsez, state_ptr);

	return (i);
}

/*
 * g723_40_decoder()
 *
 * Decodes a 5-bit CCITT G.723 40Kbps code and returns
 * the resulting 16-bit linear PCM, A-law or u-law sample value.
 * -1 is returned if the output coding is unknown.
 */
int
g723_40_decoder(
	int		i,
	int		out_coding,
	struct g72x_state *state_ptr)
{
	short		sezi, sei, sez, se;	/* ACCUM */
	short		y, dif;			/* MIX */
	short		sr;			/* ADDB */
	short		dq;
	short		dqsez;

	i &= 0x1f;			/* mask to get proper bits */
	sezi = predictor_zero(state_ptr);
	sez = sezi >> 1;
	sei = sezi + predictor_pole(state_ptr);
	se = sei >> 1;			/* se = estimated signal */

	y = step_size(state_ptr);	/* adaptive quantizer step size */
	dq = reconstruct(i & 0x10, _dqlntab[i], y);	/* estimation diff. */

	sr = (dq < 0) ? (se - (dq & 0x7FFF)) : (se + dq); /* reconst. signal */

	dqsez = sr - se + sez;		/* pole prediction diff. */

	update(5, y, _witab[i], _fitab[i], dq, sr, dqsez, state_ptr);

	switch (out_coding) {
	case AUDIO_ENCODING_ALAW:
		return (tandem_adjust_alaw(sr, se, y, i, 0x10, qtab_723_40));
	case AUDIO_ENCODING_ULAW:
		return (tandem_adjust_ulaw(sr, se, y, i, 0x10, qtab_723_40));
	case AUDIO_ENCODING_LINEAR:
		return (sr << 2);	/* sr was of 14-bit dynamic range */
	default:
		return (-1);
	}
}
sr = (dq < 0) ? (se - (dq & 0x7FFF)) : (se + dq); /* reconst. signal */

	dqsez = sr - se + sez;		/* poG711_G721_G723/g72x.c                                                                               000444  040207  037200  00000034060 05250514235 012716  0                                                                                                    ustar 00ajr                             divfstf                                                                                                                                                                                                                /*
 * This source code is a product of Sun Microsystems, Inc. and is provided
 * for unrestricted use.  Users may copy or modify this source code without
 * charge.
 *
 * SUN SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING
 * THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun source code is provided with no support and without any obligation on
 * the part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY THIS SOFTWARE
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */

/*
 * g72x.c
 *
 * Common routines for G.721 and G.723 conversions.
 */

#include "g72x.h"

static short power2[15] = {1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80,
			0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000};

/*
 * quan()
 *
 * quantizes the input val against the table of size short integers.
 * It returns i if table[i - 1] <= val < table[i].
 *
 * Using linear search for simple coding.
 */
static int
quan(
	int		val,
	short		*table,
	int		size)
{
	int		i;

	for (i = 0; i < size; i++)
		if (val < *table++)
			break;
	return (i);
}

/*
 * fmult()
 *
 * returns the integer product of the 14-bit integer "an" and
 * "floating point" representation (4-bit exponent, 6-bit mantessa) "srn".
 */
static int
fmult(
	int		an,
	int		srn)
{
	short		anmag, anexp, anmant;
	short		wanexp, wanmag, wanmant;
	short		retval;

	anmag = (an > 0) ? an : ((-an) & 0x1FFF);
	anexp = quan(anmag, power2, 15) - 6;
	anmant = (anmag == 0) ? 32 :
	    (anexp >= 0) ? anmag >> anexp : anmag << -anexp;
	wanexp = anexp + ((srn >> 6) & 0xF) - 13;

	wanmant = (anmant * (srn & 077) + 0x30) >> 4;
	retval = (wanexp >= 0) ? ((wanmant << wanexp) & 0x7FFF) :
	    (wanmant >> -wanexp);

	return (((an ^ srn) < 0) ? -retval : retval);
}

/*
 * g72x_init_state()
 *
 * This routine initializes and/or resets the g72x_state structure
 * pointed to by 'state_ptr'.
 * All the initial state values are specified in the CCITT G.721 document.
 */
void
g72x_init_state(
	struct g72x_state *state_ptr)
{
	int		cnta;

	state_ptr->yl = 34816;
	state_ptr->yu = 544;
	state_ptr->dms = 0;
	state_ptr->dml = 0;
	state_ptr->ap = 0;
	for (cnta = 0; cnta < 2; cnta++) {
		state_ptr->a[cnta] = 0;
		state_ptr->pk[cnta] = 0;
		state_ptr->sr[cnta] = 32;
	}
	for (cnta = 0; cnta < 6; cnta++) {
		state_ptr->b[cnta] = 0;
		state_ptr->dq[cnta] = 32;
	}
	state_ptr->td = 0;
}

/*
 * predictor_zero()
 *
 * computes the estimated signal from 6-zero predictor.
 *
 */
int
predictor_zero(
	struct g72x_state *state_ptr)
{
	int		i;
	int		sezi;

	sezi = fmult(state_ptr->b[0] >> 2, state_ptr->dq[0]);
	for (i = 1; i < 6; i++)			/* ACCUM */
		sezi += fmult(state_ptr->b[i] >> 2, state_ptr->dq[i]);
	return (sezi);
}
/*
 * predictor_pole()
 *
 * computes the estimated signal from 2-pole predictor.
 *
 */
int
predictor_pole(
	struct g72x_state *state_ptr)
{
	return (fmult(state_ptr->a[1] >> 2, state_ptr->sr[1]) +
	    fmult(state_ptr->a[0] >> 2, state_ptr->sr[0]));
}
/*
 * step_size()
 *
 * computes the quantization step size of the adaptive quantizer.
 *
 */
int
step_size(
	struct g72x_state *state_ptr)
{
	int		y;
	int		dif;
	int		al;

	if (state_ptr->ap >= 256)
		return (state_ptr->yu);
	else {
		y = state_ptr->yl >> 6;
		dif = state_ptr->yu - y;
		al = state_ptr->ap >> 2;
		if (dif > 0)
			y += (dif * al) >> 6;
		else if (dif < 0)
			y += (dif * al + 0x3F) >> 6;
		return (y);
	}
}

/*
 * quantize()
 *
 * Given a raw sample, 'd', of the difference signal and a
 * quantization step size scale factor, 'y', this routine returns the
 * ADPCM codeword to which that sample gets quantized.  The step
 * size scale factor division operation is done in the log base 2 domain
 * as a subtraction.
 */
int
quantize(
	int		d,	/* Raw difference signal sample */
	int		y,	/* Step size multiplier */
	short		*table,	/* quantization table */
	int		size)	/* table size of short integers */
{
	short		dqm;	/* Magnitude of 'd' */
	short		exp;	/* Integer part of base 2 log of 'd' */
	short		mant;	/* Fractional part of base 2 log */
	short		dl;	/* Log of magnitude of 'd' */
	short		dln;	/* Step size scale factor normalized log */
	int		i;

	/*
	 * LOG
	 *
	 * Compute base 2 log of 'd', and store in 'dl'.
	 */
	dqm = abs(d);
	exp = quan(dqm >> 1, power2, 15);
	mant = ((dqm << 7) >> exp) & 0x7F;	/* Fractional portion. */
	dl = (exp << 7) + mant;

	/*
	 * SUBTB
	 *
	 * "Divide" by step size multiplier.
	 */
	dln = dl - (y >> 2);

	/*
	 * QUAN
	 *
	 * Obtain codword i for 'd'.
	 */
	i = quan(dln, table, size);
	if (d < 0)			/* take 1's complement of i */
		return ((size << 1) + 1 - i);
	else if (i == 0)		/* take 1's complement of 0 */
		return ((size << 1) + 1); /* new in 1988 */
	else
		return (i);
}
/*
 * reconstruct()
 *
 * Returns reconstructed difference signal 'dq' obtained from
 * codeword 'i' and quantization step size scale factor 'y'.
 * Multiplication is performed in log base 2 domain as addition.
 */
int
reconstruct(
	int		sign,	/* 0 for non-negative value */
	int		dqln,	/* G.72x codeword */
	int		y)	/* Step size multiplier */
{
	short		dql;	/* Log of 'dq' magnitude */
	short		dex;	/* Integer part of log */
	short		dqt;
	short		dq;	/* Reconstructed difference signal sample */

	dql = dqln + (y >> 2);	/* ADDA */

	if (dql < 0) {
		return ((sign) ? -0x8000 : 0);
	} else {		/* ANTILOG */
		dex = (dql >> 7) & 15;
		dqt = 128 + (dql & 127);
		dq = (dqt << 7) >> (14 - dex);
		return ((sign) ? (dq - 0x8000) : dq);
	}
}


/*
 * update()
 *
 * updates the state variables for each output code
 */
void
update(
	int		code_size,	/* distinguish 723_40 with others */
	int		y,		/* quantizer step size */
	int		wi,		/* scale factor multiplier */
	int		fi,		/* for long/short term energies */
	int		dq,		/* quantized prediction difference */
	int		sr,		/* reconstructed signal */
	int		dqsez,		/* difference from 2-pole predictor */
	struct g72x_state *state_ptr)	/* coder state pointer */
{
	int		cnt;
	short		mag, exp, mant;	/* Adaptive predictor, FLOAT A */
	short		a2p;		/* LIMC */
	short		a1ul;		/* UPA1 */
	short		ua2, pks1;	/* UPA2 */
	short		uga2a, fa1;
	short		uga2b;
	char		tr;		/* tone/transition detector */
	short		ylint, thr2, dqthr;
	short  		ylfrac, thr1;
	short		pk0;

	pk0 = (dqsez < 0) ? 1 : 0;	/* needed in updating predictor poles */

	mag = dq & 0x7FFF;		/* prediction difference magnitude */
	/* TRANS */
	ylint = state_ptr->yl >> 15;	/* exponent part of yl */
	ylfrac = (state_ptr->yl >> 10) & 0x1F;	/* fractional part of yl */
	thr1 = (32 + ylfrac) << ylint;		/* threshold */
	thr2 = (ylint > 9) ? 31 << 10 : thr1;	/* limit thr2 to 31 << 10 */
	dqthr = (thr2 + (thr2 >> 1)) >> 1;	/* dqthr = 0.75 * thr2 */
	if (state_ptr->td == 0)		/* signal supposed voice */
		tr = 0;
	else if (mag <= dqthr)		/* supposed data, but small mag */
		tr = 0;			/* treated as voice */
	else				/* signal is data (modem) */
		tr = 1;

	/*
	 * Quantizer scale factor adaptation.
	 */

	/* FUNCTW & FILTD & DELAY */
	/* update non-steady state step size multiplier */
	state_ptr->yu = y + ((wi - y) >> 5);

	/* LIMB */
	if (state_ptr->yu < 544)	/* 544 <= yu <= 5120 */
		state_ptr->yu = 544;
	else if (state_ptr->yu > 5120)
		state_ptr->yu = 5120;

	/* FILTE & DELAY */
	/* update steady state step size multiplier */
	state_ptr->yl += state_ptr->yu + ((-state_ptr->yl) >> 6);

	/*
	 * Adaptive predictor coefficients.
	 */
	if (tr == 1) {			/* reset a's and b's for modem signal */
		state_ptr->a[0] = 0;
		state_ptr->a[1] = 0;
		state_ptr->b[0] = 0;
		state_ptr->b[1] = 0;
		state_ptr->b[2] = 0;
		state_ptr->b[3] = 0;
		state_ptr->b[4] = 0;
		state_ptr->b[5] = 0;
	} else {			/* update a's and b's */
		pks1 = pk0 ^ state_ptr->pk[0];		/* UPA2 */

		/* update predictor pole a[1] */
		a2p = state_ptr->a[1] - (state_ptr->a[1] >> 7);
		if (dqsez != 0) {
			fa1 = (pks1) ? state_ptr->a[0] : -state_ptr->a[0];
			if (fa1 < -8191)	/* a2p = function of fa1 */
				a2p -= 0x100;
			else if (fa1 > 8191)
				a2p += 0xFF;
			else
				a2p += fa1 >> 5;

			if (pk0 ^ state_ptr->pk[1])
				/* LIMC */
				if (a2p <= -12160)
					a2p = -12288;
				else if (a2p >= 12416)
					a2p = 12288;
				else
					a2p -= 0x80;
			else if (a2p <= -12416)
				a2p = -12288;
			else if (a2p >= 12160)
				a2p = 12288;
			else
				a2p += 0x80;
		}

		/* TRIGB & DELAY */
		state_ptr->a[1] = a2p;

		/* UPA1 */
		/* update predictor pole a[0] */
		state_ptr->a[0] -= state_ptr->a[0] >> 8;
		if (dqsez != 0)
			if (pks1 == 0)
				state_ptr->a[0] += 192;
			else
				state_ptr->a[0] -= 192;

		/* LIMD */
		a1ul = 15360 - a2p;
		if (state_ptr->a[0] < -a1ul)
			state_ptr->a[0] = -a1ul;
		else if (state_ptr->a[0] > a1ul)
			state_ptr->a[0] = a1ul;

		/* UPB : update predictor zeros b[6] */
		for (cnt = 0; cnt < 6; cnt++) {
			if (code_size == 5)		/* for 40Kbps G.723 */
				state_ptr->b[cnt] -= state_ptr->b[cnt] >> 9;
			else			/* for G.721 and 24Kbps G.723 */
				state_ptr->b[cnt] -= state_ptr->b[cnt] >> 8;
			if (dq & 0x7FFF) {			/* XOR */
				if ((dq ^ state_ptr->dq[cnt]) >= 0)
					state_ptr->b[cnt] += 128;
				else
					state_ptr->b[cnt] -= 128;
			}
		}
	}

	for (cnt = 5; cnt > 0; cnt--)
		state_ptr->dq[cnt] = state_ptr->dq[cnt-1];
	/* FLOAT A : convert dq[0] to 4-bit exp, 6-bit mantissa f.p. */
	if (mag == 0) {
		state_ptr->dq[0] = (dq >= 0) ? 0x20 : 0xFC20;
	} else {
		exp = quan(mag, power2, 15);
		state_ptr->dq[0] = (dq >= 0) ?
		    (exp << 6) + ((mag << 6) >> exp) :
		    (exp << 6) + ((mag << 6) >> exp) - 0x400;
	}

	state_ptr->sr[1] = state_ptr->sr[0];
	/* FLOAT B : convert sr to 4-bit exp., 6-bit mantissa f.p. */
	if (sr == 0) {
		state_ptr->sr[0] = 0x20;
	} else if (sr > 0) {
		exp = quan(sr, power2, 15);
		state_ptr->sr[0] = (exp << 6) + ((sr << 6) >> exp);
	} else if (sr > -32768) {
		mag = -sr;
		exp = quan(mag, power2, 15);
		state_ptr->sr[0] =  (exp << 6) + ((mag << 6) >> exp) - 0x400;
	} else
		state_ptr->sr[0] = 0xFC20;

	/* DELAY A */
	state_ptr->pk[1] = state_ptr->pk[0];
	state_ptr->pk[0] = pk0;

	/* TONE */
	if (tr == 1)		/* this sample has been treated as data */
		state_ptr->td = 0;	/* next one will be treated as voice */
	else if (a2p < -11776)	/* small sample-to-sample correlation */
		state_ptr->td = 1;	/* signal may be data */
	else				/* signal is voice */
		state_ptr->td = 0;

	/*
	 * Adaptation speed control.
	 */
	state_ptr->dms += (fi - state_ptr->dms) >> 5;		/* FILTA */
	state_ptr->dml += (((fi << 2) - state_ptr->dml) >> 7);	/* FILTB */

	if (tr == 1)
		state_ptr->ap = 256;
	else if (y < 1536)					/* SUBTC */
		state_ptr->ap += (0x200 - state_ptr->ap) >> 4;
	else if (state_ptr->td == 1)
		state_ptr->ap += (0x200 - state_ptr->ap) >> 4;
	else if (abs((state_ptr->dms << 2) - state_ptr->dml) >=
	    (state_ptr->dml >> 3))
		state_ptr->ap += (0x200 - state_ptr->ap) >> 4;
	else
		state_ptr->ap += (-state_ptr->ap) >> 4;
}

/*
 * tandem_adjust(sr, se, y, i, sign)
 *
 * At the end of ADPCM decoding, it simulates an encoder which may be receiving
 * the output of this decoder as a tandem process. If the output of the
 * simulated encoder differs from the input to this decoder, the decoder output
 * is adjusted by one level of A-law or u-law codes.
 *
 * Input:
 *	sr	decoder output linear PCM sample,
 *	se	predictor estimate sample,
 *	y	quantizer step size,
 *	i	decoder input code,
 *	sign	sign bit of code i
 *
 * Return:
 *	adjusted A-law or u-law compressed sample.
 */
int
tandem_adjust_alaw(
	int		sr,	/* decoder output linear PCM sample */
	int		se,	/* predictor estimate sample */
	int		y,	/* quantizer step size */
	int		i,	/* decoder input code */
	int		sign,
	short		*qtab)
{
	unsigned char	sp;	/* A-law compressed 8-bit code */
	short		dx;	/* prediction error */
	char		id;	/* quantized prediction error */
	int		sd;	/* adjusted A-law decoded sample value */
	int		im;	/* biased magnitude of i */
	int		imx;	/* biased magnitude of id */

	if (sr <= -32768)
		sr = -1;
	sp = linear2alaw((sr >> 1) << 3);	/* short to A-law compression */
	dx = (alaw2linear(sp) >> 2) - se;	/* 16-bit prediction error */
	id = quantize(dx, y, qtab, sign - 1);

	if (id == i) {			/* no adjustment on sp */
		return (sp);
	} else {			/* sp adjustment needed */
		/* ADPCM codes : 8, 9, ... F, 0, 1, ... , 6, 7 */
		im = i ^ sign;		/* 2's complement to biased unsigned */
		imx = id ^ sign;

		if (imx > im) {		/* sp adjusted to next lower value */
			if (sp & 0x80) {
				sd = (sp == 0xD5) ? 0x55 :
				    ((sp ^ 0x55) - 1) ^ 0x55;
			} else {
				sd = (sp == 0x2A) ? 0x2A :
				    ((sp ^ 0x55) + 1) ^ 0x55;
			}
		} else {		/* sp adjusted to next higher value */
			if (sp & 0x80)
				sd = (sp == 0xAA) ? 0xAA :
				    ((sp ^ 0x55) + 1) ^ 0x55;
			else
				sd = (sp == 0x55) ? 0xD5 :
				    ((sp ^ 0x55) - 1) ^ 0x55;
		}
		return (sd);
	}
}

int
tandem_adjust_ulaw(
	int		sr,	/* decoder output linear PCM sample */
	int		se,	/* predictor estimate sample */
	int		y,	/* quantizer step size */
	int		i,	/* decoder input code */
	int		sign,
	short		*qtab)
{
	unsigned char	sp;	/* u-law compressed 8-bit code */
	short		dx;	/* prediction error */
	char		id;	/* quantized prediction error */
	int		sd;	/* adjusted u-law decoded sample value */
	int		im;	/* biased magnitude of i */
	int		imx;	/* biased magnitude of id */

	if (sr <= -32768)
		sr = 0;
	sp = linear2ulaw(sr << 2);	/* short to u-law compression */
	dx = (ulaw2linear(sp) >> 2) - se;	/* 16-bit prediction error */
	id = quantize(dx, y, qtab, sign - 1);
	if (id == i) {
		return (sp);
	} else {
		/* ADPCM codes : 8, 9, ... F, 0, 1, ... , 6, 7 */
		im = i ^ sign;		/* 2's complement to biased unsigned */
		imx = id ^ sign;
		if (imx > im) {		/* sp adjusted to next lower value */
			if (sp & 0x80)
				sd = (sp == 0xFF) ? 0x7E : sp + 1;
			else
				sd = (sp == 0) ? 0 : sp - 1;

		} else {		/* sp adjusted to next higher value */
			if (sp & 0x80)
				sd = (sp == 0x80) ? 0x80 : sp - 1;
			else
				sd = (sp == 0x7F) ? 0xFE : sp + 1;
		}
		return (sd);
	}
}
 - 1);
	if (id == i) {
		return (sp);
	} else {
		/* ADPCM codes : 8, 9, ... F, 0, 1, ... , 6, 7 */
		im = i ^ sign;		/* 2's complement to biased unsigned */
		imx = id ^ sign;
		if (imx > im) {		/* sp adjusted to next lower value */
			if (sp & 0x80)
				sd = (sp == 0xFF) ? 0x7E : sp + 1;
			else
				sd = (sp == 0) ? 0 : sp - 1;

		} else {		/* sp adjusted to next higher value */
			if (sp & 0x80)
				sd = (sp == 0x80) ? 0x80 : sp - 1;
			else
				sd = (sp ==G711_G721_G723/g72x.h                                                                               000444  040207  037200  00000006407 05250514235 012727  0                                                                                                    ustar 00ajr                             divfstf                                                                                                                                                                                                                /*
 * This source code is a product of Sun Microsystems, Inc. and is provided
 * for unrestricted use.  Users may copy or modify this source code without
 * charge.
 *
 * SUN SOURCE CODE IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING
 * THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun source code is provided with no support and without any obligation on
 * the part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY THIS SOFTWARE
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */

/*
 * g72x.h
 *
 * Header file for CCITT conversion routines.
 *
 */
#ifndef _G72X_H
#define	_G72X_H

#define	AUDIO_ENCODING_ULAW	(1)	/* ISDN u-law */
#define	AUDIO_ENCODING_ALAW	(2)	/* ISDN A-law */
#define	AUDIO_ENCODING_LINEAR	(3)	/* PCM 2's-complement (0-center) */

/*
 * The following is the definition of the state structure
 * used by the G.721/G.723 encoder and decoder to preserve their internal
 * state between successive calls.  The meanings of the majority
 * of the state structure fields are explained in detail in the
 * CCITT Recommendation G.721.  The field names are essentially indentical
 * to variable names in the bit level description of the coding algorithm
 * included in this Recommendation.
 */
struct g72x_state {
	long yl;	/* Locked or steady state step size multiplier. */
	short yu;	/* Unlocked or non-steady state step size multiplier. */
	short dms;	/* Short term energy estimate. */
	short dml;	/* Long term energy estimate. */
	short ap;	/* Linear weighting coefficient of 'yl' and 'yu'. */

	short a[2];	/* Coefficients of pole portion of prediction filter. */
	short b[6];	/* Coefficients of zero portion of prediction filter. */
	short pk[2];	/*
			 * Signs of previous two samples of a partially
			 * reconstructed signal.
			 */
	short dq[6];	/*
			 * Previous 6 samples of the quantized difference
			 * signal represented in an internal floating point
			 * format.
			 */
	short sr[2];	/*
			 * Previous 2 samples of the quantized difference
			 * signal represented in an internal floating point
			 * format.
			 */
	char td;	/* delayed tone detect, new in 1988 version */
};

/* External function definitions. */

extern void g72x_init_stat(struct g72x_state *);
extern int g721_encoder(
		int sample,
		int in_coding,
		struct g72x_state *state_ptr);
extern int g721_decoder(
		int code,
		int out_coding,
		struct g72x_state *state_ptr);
extern int g723_24_encoder(
		int sample,
		int in_coding,
		struct g72x_state *state_ptr);
extern int g723_24_decoder(
		int code,
		int out_coding,
		struct g72x_state *state_ptr);
extern int g723_40_encoder(
		int sample,
		int in_coding,
		struct g72x_state *state_ptr);
extern int g723_40_decoder(
		int code,
		int out_coding,
		struct g72x_state *state_ptr);

#endif /* !_G72X_H */
,
		struct g72x_state *state_ptr);
extern int g721_decoder(
		int code,
		int out_coding,
		struct g72x_state *state_ptr);
extern int g723_24_encoder(
		int sample,
		int in_coding,
		struct g72x_state *state_ptr);
extern int g723_24_decoder(
		int                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 