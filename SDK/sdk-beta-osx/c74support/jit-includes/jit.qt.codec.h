#ifndef __JIT_QT_CODEC_H__
#define __JIT_QT_CODEC_H__

void jit_qt_codec_gensymbols(void);
long jit_qt_codec_sym2qual(t_symbol *s); 
long jit_qt_codec_sym2type(t_symbol *s); 
t_symbol *jit_qt_codec_qual2sym(long qual); 
t_symbol *jit_qt_codec_type2sym(long type); 
long jit_qt_codec_sym2acodec(t_symbol *s); 
t_symbol *jit_qt_codec_acodec2sym(long acodec); 

/*
//codec types;
raw, cinepak, graphics, animation, video, componentvideo, jpeg, mjpega, 
mjpegb, sgi, planarrgb, macpaint, gif, photocd, qdgx, avrjpeg, opendmljpeg, 
bmp, winraw, vector, qd, h261, h263, dvntsc, dvpal, dvprontsc, dvpropal, flc, 
targa, png, tiff, componentvideosigned, componentvideounsigned, cmyk, 
microsoft, sorenson, indeo4, argb64, rgb48, alphagrey32, grey16, mpegyuv420, 
yuv420, sorensonyuv9;
;
//codec qualities;
lossless, max, min, low, normal, high;
;
//audio codecs;
none, raw, big16, little16, float32, float64, 24bit, 32bit, 
little32, mace3, mace6, cdxa4, cdxa2, ima, ulaw, alaw, adpcm, 
dviima, dvaudio, qdesign, qdesign2, qualcomm, mp3;
*/

#endif //__JIT_QT_CODEC_H__