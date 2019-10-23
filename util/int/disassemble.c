/** @file
 *  For disassembling the text segment.
 */

/* $Id$ */

#include	"global.h"
#include	"opcode.h"
#include	"memdirect.h"
#include	"proctab.h"
#include	"alloc.h"
#include	"whatever.h"

PRIVATE ptr TC;
PRIVATE void do_pr_instr(unsigned int);

/* This text is copied and modified from text.h */

#define	text_loc(a)	(*(text + (p2i(a))))

/*	Reading the opcode.
 */
#define	nextTCbyte()	(TC+=1, btou(text_loc(TC-1)))

/*	Shortie arguments consist of the high order value, derived from
 the opcode and passed as a parameter, and the following byte.
 */
#define	St_arg(h)	(TC+=1, ((h)<<8) + btol(text_loc(TC-1)))

/*	Two-byte arguments consist of the following two bytes.
 */

#define	Lt_arg_2()	(TC+=2, (btol(text_loc(TC-1)) | \
				(btos(text_loc(TC-2)) << 8)))

#define	Pt_arg_2()	(TC+=2, (btol(text_loc(TC-1)) | \
				(btos(text_loc(TC-2)) << 8)))/* should test */

#define	Nt_arg_2()	(TC+=2, (btol(text_loc(TC-1)) | \
				(btos(text_loc(TC-2)) << 8)))/* should test */

#define	Ut_arg()		(TC+=2, (btol(text_loc(TC-1)) | \
				(btol(text_loc(TC-2)) << 8)))

/*	The L-, P-, and N-4-bytes #defines are all equal, because
 we assume our longs to be 4 bytes long.
 */

#define	Lt_arg_4()	(TC+=4, (btol(text_loc(TC-1)) | \
				(btol(text_loc(TC-2)) << 8) | \
				(btol(text_loc(TC-3)) << 16) | \
				(btos(text_loc(TC-4)) << 24)))

#define	Pt_arg_4()	(TC+=4, (btol(text_loc(TC-1)) | \
				(btol(text_loc(TC-2)) << 8) | \
				(btol(text_loc(TC-3)) << 16) | \
				(btos(text_loc(TC-4)) << 24)))/* should test */

#define	Nt_arg_4()	(TC+=4, (btol(text_loc(TC-1)) | \
				(btol(text_loc(TC-2)) << 8) | \
				(btol(text_loc(TC-3)) << 16) | \
				(btos(text_loc(TC-4)) << 24)))/* should test */

/* This text was generated by mkswitch Pr and then modified */

PRIVATE void PrAARl2(long arg)
{
	printf(" AAR %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrAARm(long arg)
{
	printf(" AAR %ld\n", arg);
}

PRIVATE void PrAARz(void)
{
	printf(" AAR\n");
}

PRIVATE void PrADFl2(long arg)
{
	printf(" ADF %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrADFs(long hob, size wfac)
{
	printf(" ADF %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrADFz(void)
{
	printf(" ADF\n");
}

PRIVATE void PrADIl2(long arg)
{
	printf(" ADI %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrADIm(long arg)
{
	printf(" ADI %ld\n", arg);
}

PRIVATE void PrADIz(void)
{
	printf(" ADI\n");
}

PRIVATE void PrADPl2(long arg)
{
	printf(" ADP %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrADPl4(long arg)
{
	printf(" ADP %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrADPm(long arg)
{
	printf(" ADP %ld\n", arg);
}

PRIVATE void PrADPs(long hob, size wfac)
{
	printf(" ADP %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrADSl2(long arg)
{
	printf(" ADS %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrADSm(long arg)
{
	printf(" ADS %ld\n", arg);
}

PRIVATE void PrADSz(void)
{
	printf(" ADS\n");
}

PRIVATE void PrADUl2(long arg)
{
	printf(" ADU %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrADUz(void)
{
	printf(" ADU\n");
}

PRIVATE void PrANDl2(long arg)
{
	printf(" AND %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrANDm(long arg)
{
	printf(" AND %ld\n", arg);
}

PRIVATE void PrANDz(void)
{
	printf(" AND\n");
}

PRIVATE void PrASPl2(long arg)
{
	printf(" ASP %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrASPl4(long arg)
{
	printf(" ASP %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrASPm(long arg)
{
	printf(" ASP %ld\n", arg);
}

PRIVATE void PrASPs(long hob, size wfac)
{
	printf(" ASP %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrASSl2(long arg)
{
	printf(" ASS %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrASSz(void)
{
	printf(" ASS\n");
}

PRIVATE void PrBEQl2(long arg)
{
	printf(" BEQ %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrBEQl4(long arg)
{
	printf(" BEQ %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrBEQs(long hob, size wfac)
{
	printf(" BEQ %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrBGEl2(long arg)
{
	printf(" BGE %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrBGEl4(long arg)
{
	printf(" BGE %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrBGEs(long hob, size wfac)
{
	printf(" BGE %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrBGTl2(long arg)
{
	printf(" BGT %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrBGTl4(long arg)
{
	printf(" BGT %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrBGTs(long hob, size wfac)
{
	printf(" BGT %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrBLEl2(long arg)
{
	printf(" BLE %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrBLEl4(long arg)
{
	printf(" BLE %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrBLEs(long hob, size wfac)
{
	printf(" BLE %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrBLMl2(long arg)
{
	printf(" BLM %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrBLMl4(long arg)
{
	printf(" BLM %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrBLMs(long hob, size wfac)
{
	printf(" BLM %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrBLSl2(long arg)
{
	printf(" BLS %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrBLSz(void)
{
	printf(" BLS\n");
}

PRIVATE void PrBLTl2(long arg)
{
	printf(" BLT %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrBLTl4(long arg)
{
	printf(" BLT %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrBLTs(long hob, size wfac)
{
	printf(" BLT %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrBNEl2(long arg)
{
	printf(" BNE %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrBNEl4(long arg)
{
	printf(" BNE %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrBNEs(long hob, size wfac)
{
	printf(" BNE %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrBRAl2(long arg)
{
	printf(" BRA %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrBRAl4(long arg)
{
	printf(" BRA %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrBRAs(long hob, size wfac)
{
	printf(" BRA %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrCAIz(void)
{
	printf(" CAI\n");
}

PRIVATE void PrCALl2(long arg)
{
	printf(" CAL %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrCALl4(long arg)
{
	printf(" CAL %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrCALm(long arg)
{
	printf(" CAL %ld\n", arg);
}

PRIVATE void PrCALs(long hob, size wfac)
{
	printf(" CAL %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrCFFz(void)
{
	printf(" CFF\n");
}

PRIVATE void PrCFIz(void)
{
	printf(" CFI\n");
}

PRIVATE void PrCFUz(void)
{
	printf(" CFU\n");
}

PRIVATE void PrCIFz(void)
{
	printf(" CIF\n");
}

PRIVATE void PrCIIz(void)
{
	printf(" CII\n");
}

PRIVATE void PrCIUz(void)
{
	printf(" CIU\n");
}

PRIVATE void PrCMFl2(long arg)
{
	printf(" CMF %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrCMFs(long hob, size wfac)
{
	printf(" CMF %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrCMFz(void)
{
	printf(" CMF\n");
}

PRIVATE void PrCMIl2(long arg)
{
	printf(" CMI %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrCMIm(long arg)
{
	printf(" CMI %ld\n", arg);
}

PRIVATE void PrCMIz(void)
{
	printf(" CMI\n");
}

PRIVATE void PrCMPz(void)
{
	printf(" CMP\n");
}

PRIVATE void PrCMSl2(long arg)
{
	printf(" CMS %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrCMSs(long hob, size wfac)
{
	printf(" CMS %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrCMSz(void)
{
	printf(" CMS\n");
}

PRIVATE void PrCMUl2(long arg)
{
	printf(" CMU %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrCMUz(void)
{
	printf(" CMU\n");
}

PRIVATE void PrCOMl2(long arg)
{
	printf(" COM %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrCOMz(void)
{
	printf(" COM\n");
}

PRIVATE void PrCSAl2(long arg)
{
	printf(" CSA %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrCSAm(long arg)
{
	printf(" CSA %ld\n", arg);
}

PRIVATE void PrCSAz(void)
{
	printf(" CSA\n");
}

PRIVATE void PrCSBl2(long arg)
{
	printf(" CSB %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrCSBm(long arg)
{
	printf(" CSB %ld\n", arg);
}

PRIVATE void PrCSBz(void)
{
	printf(" CSB\n");
}

PRIVATE void PrCUFz(void)
{
	printf(" CUF\n");
}

PRIVATE void PrCUIz(void)
{
	printf(" CUI\n");
}

PRIVATE void PrCUUz(void)
{
	printf(" CUU\n");
}

PRIVATE void PrDCHz(void)
{
	printf(" DCH\n");
}

PRIVATE void PrDECz(void)
{
	printf(" DEC\n");
}

PRIVATE void PrDEEl2(long arg)
{
	printf(" DEE %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrDEEl4(long arg)
{
	printf(" DEE %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrDEEs(long hob, size wfac)
{
	printf(" DEE %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrDELn2(long arg)
{
	printf(" DEL %ld\n", Nt_arg_2() * arg);
}

PRIVATE void PrDELn4(long arg)
{
	printf(" DEL %ld\n", Nt_arg_4() * arg);
}

PRIVATE void PrDELp2(long arg)
{
	printf(" DEL %ld\n", Pt_arg_2() * arg);
}

PRIVATE void PrDELp4(long arg)
{
	printf(" DEL %ld\n", Pt_arg_4() * arg);
}

PRIVATE void PrDELs(long hob, size wfac)
{
	printf(" DEL %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrDUPl2(long arg)
{
	printf(" DUP %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrDUPm(long arg)
{
	printf(" DUP %ld\n", arg);
}

PRIVATE void PrDUSl2(long arg)
{
	printf(" DUS %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrDUSz(void)
{
	printf(" DUS\n");
}

PRIVATE void PrDVFl2(long arg)
{
	printf(" DVF %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrDVFs(long hob, size wfac)
{
	printf(" DVF %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrDVFz(void)
{
	printf(" DVF\n");
}

PRIVATE void PrDVIl2(long arg)
{
	printf(" DVI %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrDVIm(long arg)
{
	printf(" DVI %ld\n", arg);
}

PRIVATE void PrDVIz(void)
{
	printf(" DVI\n");
}

PRIVATE void PrDVUl2(long arg)
{
	printf(" DVU %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrDVUz(void)
{
	printf(" DVU\n");
}

PRIVATE void PrEXGl2(long arg)
{
	printf(" EXG %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrEXGs(long hob, size wfac)
{
	printf(" EXG %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrEXGz(void)
{
	printf(" EXG\n");
}

PRIVATE void PrFEFl2(long arg)
{
	printf(" FEF %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrFEFz(void)
{
	printf(" FEF\n");
}

PRIVATE void PrFIFl2(long arg)
{
	printf(" FIF %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrFIFz(void)
{
	printf(" FIF\n");
}

PRIVATE void PrFILu(long arg)
{
	printf(" FIL %ld\n", Ut_arg() * arg);
}

PRIVATE void PrFILl4(long arg)
{
	printf(" FIL %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrGTOu(long arg)
{
	printf(" GTO %ld\n", Ut_arg() * arg);
}

PRIVATE void PrGTOl4(long arg)
{
	printf(" GTO %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrINCz(void)
{
	printf(" INC\n");
}

PRIVATE void PrINEl2(long arg)
{
	printf(" INE %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrINEl4(long arg)
{
	printf(" INE %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrINEs(long hob, size wfac)
{
	printf(" INE %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrINLm(long arg)
{
	printf(" INL %ld\n", arg);
}

PRIVATE void PrINLn2(long arg)
{
	printf(" INL %ld\n", Nt_arg_2() * arg);
}

PRIVATE void PrINLn4(long arg)
{
	printf(" INL %ld\n", Nt_arg_4() * arg);
}

PRIVATE void PrINLp2(long arg)
{
	printf(" INL %ld\n", Pt_arg_2() * arg);
}

PRIVATE void PrINLp4(long arg)
{
	printf(" INL %ld\n", Pt_arg_4() * arg);
}

PRIVATE void PrINLs(long hob, size wfac)
{
	printf(" INL %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrINNl2(long arg)
{
	printf(" INN %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrINNs(long hob, size wfac)
{
	printf(" INN %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrINNz(void)
{
	printf(" INN\n");
}

PRIVATE void PrIORl2(long arg)
{
	printf(" IOR %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrIORm(long arg)
{
	printf(" IOR %ld\n", arg);
}

PRIVATE void PrIORs(long hob, size wfac)
{
	printf(" IOR %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrIORz(void)
{
	printf(" IOR\n");
}

PRIVATE void PrLAEu(long arg)
{
	printf(" LAE %ld\n", Ut_arg() * arg);
}

PRIVATE void PrLAEl4(long arg)
{
	printf(" LAE %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrLAEs(long hob, size wfac)
{
	printf(" LAE %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrLALm(long arg)
{
	printf(" LAL %ld\n", arg);
}

PRIVATE void PrLALn2(long arg)
{
	printf(" LAL %ld\n", Nt_arg_2() * arg);
}

PRIVATE void PrLALn4(long arg)
{
	printf(" LAL %ld\n", Nt_arg_4() * arg);
}

PRIVATE void PrLALp2(long arg)
{
	printf(" LAL %ld\n", Pt_arg_2() * arg);
}

PRIVATE void PrLALp4(long arg)
{
	printf(" LAL %ld\n", Pt_arg_4() * arg);
}

PRIVATE void PrLALs(long hob, size wfac)
{
	printf(" LAL %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrLARl2(long arg)
{
	printf(" LAR %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrLARm(long arg)
{
	printf(" LAR %ld\n", arg);
}

PRIVATE void PrLARz(void)
{
	printf(" LAR\n");
}

PRIVATE void PrLDCl2(long arg)
{
	printf(" LDC %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrLDCl4(long arg)
{
	printf(" LDC %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrLDCm(long arg)
{
	printf(" LDC %ld\n", arg);
}

PRIVATE void PrLDEl2(long arg)
{
	printf(" LDE %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrLDEl4(long arg)
{
	printf(" LDE %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrLDEs(long hob, size wfac)
{
	printf(" LDE %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrLDFl2(long arg)
{
	printf(" LDF %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrLDFl4(long arg)
{
	printf(" LDF %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrLDLm(long arg)
{
	printf(" LDL %ld\n", arg);
}

PRIVATE void PrLDLn2(long arg)
{
	printf(" LDL %ld\n", Nt_arg_2() * arg);
}

PRIVATE void PrLDLn4(long arg)
{
	printf(" LDL %ld\n", Nt_arg_4() * arg);
}

PRIVATE void PrLDLp2(long arg)
{
	printf(" LDL %ld\n", Pt_arg_2() * arg);
}

PRIVATE void PrLDLp4(long arg)
{
	printf(" LDL %ld\n", Pt_arg_4() * arg);
}

PRIVATE void PrLDLs(long hob, size wfac)
{
	printf(" LDL %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrLFRl2(long arg)
{
	printf(" LFR %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrLFRm(long arg)
{
	printf(" LFR %ld\n", arg);
}

PRIVATE void PrLFRs(long hob, size wfac)
{
	printf(" LFR %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrLILm(long arg)
{
	printf(" LIL %ld\n", arg);
}

PRIVATE void PrLILn2(long arg)
{
	printf(" LIL %ld\n", Nt_arg_2() * arg);
}

PRIVATE void PrLILn4(long arg)
{
	printf(" LIL %ld\n", Nt_arg_4() * arg);
}

PRIVATE void PrLILp2(long arg)
{
	printf(" LIL %ld\n", Pt_arg_2() * arg);
}

PRIVATE void PrLILp4(long arg)
{
	printf(" LIL %ld\n", Pt_arg_4() * arg);
}

PRIVATE void PrLILs(long hob, size wfac)
{
	printf(" LIL %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrLIMz(void)
{
	printf(" LIM\n");
}

PRIVATE void PrLINl2(long arg)
{
	printf(" LIN %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrLINl4(long arg)
{
	printf(" LIN %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrLINs(long hob, size wfac)
{
	printf(" LIN %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrLNIz(void)
{
	printf(" LNI\n");
}

PRIVATE void PrLOCl2(long arg)
{
	printf(" LOC %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrLOCl4(long arg)
{
	printf(" LOC %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrLOCm(long arg)
{
	printf(" LOC %ld\n", arg);
}

PRIVATE void PrLOCs(long hob, size wfac)
{
	printf(" LOC %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrLOEl2(long arg)
{
	printf(" LOE %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrLOEl4(long arg)
{
	printf(" LOE %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrLOEs(long hob, size wfac)
{
	printf(" LOE %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrLOFl2(long arg)
{
	printf(" LOF %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrLOFl4(long arg)
{
	printf(" LOF %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrLOFm(long arg)
{
	printf(" LOF %ld\n", arg);
}

PRIVATE void PrLOFs(long hob, size wfac)
{
	printf(" LOF %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrLOIl2(long arg)
{
	printf(" LOI %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrLOIl4(long arg)
{
	printf(" LOI %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrLOIm(long arg)
{
	printf(" LOI %ld\n", arg);
}

PRIVATE void PrLOIs(long hob, size wfac)
{
	printf(" LOI %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrLOLm(long arg)
{
	printf(" LOL %ld\n", arg);
}

PRIVATE void PrLOLn2(long arg)
{
	printf(" LOL %ld\n", Nt_arg_2() * arg);
}

PRIVATE void PrLOLn4(long arg)
{
	printf(" LOL %ld\n", Nt_arg_4() * arg);
}

PRIVATE void PrLOLp2(long arg)
{
	printf(" LOL %ld\n", Pt_arg_2() * arg);
}

PRIVATE void PrLOLp4(long arg)
{
	printf(" LOL %ld\n", Pt_arg_4() * arg);
}

PRIVATE void PrLOLs(long hob, size wfac)
{
	printf(" LOL %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrLORs(long hob, size wfac)
{
	printf(" LOR %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrLOSl2(long arg)
{
	printf(" LOS %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrLOSz(void)
{
	printf(" LOS\n");
}

PRIVATE void PrLPBz(void)
{
	printf(" LPB\n");
}

PRIVATE void PrLPIl2(long arg)
{
	printf(" LPI %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrLPIl4(long arg)
{
	printf(" LPI %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrLXAl2(long arg)
{
	printf(" LXA %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrLXAm(long arg)
{
	printf(" LXA %ld\n", arg);
}

PRIVATE void PrLXLl2(long arg)
{
	printf(" LXL %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrLXLm(long arg)
{
	printf(" LXL %ld\n", arg);
}

PRIVATE void PrMLFl2(long arg)
{
	printf(" MLF %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrMLFs(long hob, size wfac)
{
	printf(" MLF %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrMLFz(void)
{
	printf(" MLF\n");
}

PRIVATE void PrMLIl2(long arg)
{
	printf(" MLI %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrMLIm(long arg)
{
	printf(" MLI %ld\n", arg);
}

PRIVATE void PrMLIz(void)
{
	printf(" MLI\n");
}

PRIVATE void PrMLUl2(long arg)
{
	printf(" MLU %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrMLUz(void)
{
	printf(" MLU\n");
}

PRIVATE void PrMONz(void)
{
	printf(" MON\n");
}

PRIVATE void PrNGFl2(long arg)
{
	printf(" NGF %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrNGFz(void)
{
	printf(" NGF\n");
}

PRIVATE void PrNGIl2(long arg)
{
	printf(" NGI %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrNGIz(void)
{
	printf(" NGI\n");
}

PRIVATE void PrNOPz(void)
{
	printf(" NOP\n");
}

PRIVATE void PrRCKl2(long arg)
{
	printf(" RCK %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrRCKm(long arg)
{
	printf(" RCK %ld\n", arg);
}

PRIVATE void PrRCKz(void)
{
	printf(" RCK\n");
}

PRIVATE void PrRETl2(long arg)
{
	printf(" RET %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrRETm(long arg)
{
	printf(" RET %ld\n", arg);
}

PRIVATE void PrRETs(long hob, size wfac)
{
	printf(" RET %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrRMIl2(long arg)
{
	printf(" RMI %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrRMIm(long arg)
{
	printf(" RMI %ld\n", arg);
}

PRIVATE void PrRMIz(void)
{
	printf(" RMI\n");
}

PRIVATE void PrRMUl2(long arg)
{
	printf(" RMU %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrRMUz(void)
{
	printf(" RMU\n");
}

PRIVATE void PrROLl2(long arg)
{
	printf(" ROL %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrROLz(void)
{
	printf(" ROL\n");
}

PRIVATE void PrRORl2(long arg)
{
	printf(" ROR %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrRORz(void)
{
	printf(" ROR\n");
}

PRIVATE void PrRTTz(void)
{
	printf(" RTT\n");
}

PRIVATE void PrSARl2(long arg)
{
	printf(" SAR %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrSARm(long arg)
{
	printf(" SAR %ld\n", arg);
}

PRIVATE void PrSARz(void)
{
	printf(" SAR\n");
}

PRIVATE void PrSBFl2(long arg)
{
	printf(" SBF %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrSBFs(long hob, size wfac)
{
	printf(" SBF %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrSBFz(void)
{
	printf(" SBF\n");
}

PRIVATE void PrSBIl2(long arg)
{
	printf(" SBI %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrSBIm(long arg)
{
	printf(" SBI %ld\n", arg);
}

PRIVATE void PrSBIz(void)
{
	printf(" SBI\n");
}

PRIVATE void PrSBSl2(long arg)
{
	printf(" SBS %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrSBSz(void)
{
	printf(" SBS\n");
}

PRIVATE void PrSBUl2(long arg)
{
	printf(" SBU %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrSBUz(void)
{
	printf(" SBU\n");
}

PRIVATE void PrSDEu(long arg)
{
	printf(" SDE %ld\n", Ut_arg() * arg);
}

PRIVATE void PrSDEl4(long arg)
{
	printf(" SDE %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrSDFl2(long arg)
{
	printf(" SDF %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrSDFl4(long arg)
{
	printf(" SDF %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrSDLn2(long arg)
{
	printf(" SDL %ld\n", Nt_arg_2() * arg);
}

PRIVATE void PrSDLn4(long arg)
{
	printf(" SDL %ld\n", Nt_arg_4() * arg);
}

PRIVATE void PrSDLp2(long arg)
{
	printf(" SDL %ld\n", Pt_arg_2() * arg);
}

PRIVATE void PrSDLp4(long arg)
{
	printf(" SDL %ld\n", Pt_arg_4() * arg);
}

PRIVATE void PrSDLs(long hob, size wfac)
{
	printf(" SDL %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrSETl2(long arg)
{
	printf(" SET %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrSETs(long hob, size wfac)
{
	printf(" SET %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrSETz(void)
{
	printf(" SET\n");
}

PRIVATE void PrSIGz(void)
{
	printf(" SIG\n");
}

PRIVATE void PrSILn2(long arg)
{
	printf(" SIL %ld\n", Nt_arg_2() * arg);
}

PRIVATE void PrSILn4(long arg)
{
	printf(" SIL %ld\n", Nt_arg_4() * arg);
}

PRIVATE void PrSILp2(long arg)
{
	printf(" SIL %ld\n", Pt_arg_2() * arg);
}

PRIVATE void PrSILp4(long arg)
{
	printf(" SIL %ld\n", Pt_arg_4() * arg);
}

PRIVATE void PrSILs(long hob, size wfac)
{
	printf(" SIL %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrSIMz(void)
{
	printf(" SIM\n");
}

PRIVATE void PrSLIl2(long arg)
{
	printf(" SLI %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrSLIm(long arg)
{
	printf(" SLI %ld\n", arg);
}

PRIVATE void PrSLIz(void)
{
	printf(" SLI\n");
}

PRIVATE void PrSLUl2(long arg)
{
	printf(" SLU %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrSLUz(void)
{
	printf(" SLU\n");
}

PRIVATE void PrSRIl2(long arg)
{
	printf(" SRI %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrSRIz(void)
{
	printf(" SRI\n");
}

PRIVATE void PrSRUl2(long arg)
{
	printf(" SRU %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrSRUz(void)
{
	printf(" SRU\n");
}

PRIVATE void PrSTEl2(long arg)
{
	printf(" STE %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrSTEl4(long arg)
{
	printf(" STE %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrSTEs(long hob, size wfac)
{
	printf(" STE %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrSTFl2(long arg)
{
	printf(" STF %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrSTFl4(long arg)
{
	printf(" STF %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrSTFm(long arg)
{
	printf(" STF %ld\n", arg);
}

PRIVATE void PrSTFs(long hob, size wfac)
{
	printf(" STF %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrSTIl2(long arg)
{
	printf(" STI %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrSTIl4(long arg)
{
	printf(" STI %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrSTIm(long arg)
{
	printf(" STI %ld\n", arg);
}

PRIVATE void PrSTIs(long hob, size wfac)
{
	printf(" STI %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrSTLm(long arg)
{
	printf(" STL %ld\n", arg);
}

PRIVATE void PrSTLn2(long arg)
{
	printf(" STL %ld\n", Nt_arg_2() * arg);
}

PRIVATE void PrSTLn4(long arg)
{
	printf(" STL %ld\n", Nt_arg_4() * arg);
}

PRIVATE void PrSTLp2(long arg)
{
	printf(" STL %ld\n", Pt_arg_2() * arg);
}

PRIVATE void PrSTLp4(long arg)
{
	printf(" STL %ld\n", Pt_arg_4() * arg);
}

PRIVATE void PrSTLs(long hob, size wfac)
{
	printf(" STL %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrSTRs(long hob, size wfac)
{
	printf(" STR %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrSTSl2(long arg)
{
	printf(" STS %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrSTSz(void)
{
	printf(" STS\n");
}

PRIVATE void PrTEQz(void)
{
	printf(" TEQ\n");
}

PRIVATE void PrTGEz(void)
{
	printf(" TGE\n");
}

PRIVATE void PrTGTz(void)
{
	printf(" TGT\n");
}

PRIVATE void PrTLEz(void)
{
	printf(" TLE\n");
}

PRIVATE void PrTLTz(void)
{
	printf(" TLT\n");
}

PRIVATE void PrTNEz(void)
{
	printf(" TNE\n");
}

PRIVATE void PrTRPz(void)
{
	printf(" TRP\n");
}

PRIVATE void PrXORl2(long arg)
{
	printf(" XOR %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrXORz(void)
{
	printf(" XOR\n");
}

PRIVATE void PrZEQl2(long arg)
{
	printf(" ZEQ %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrZEQl4(long arg)
{
	printf(" ZEQ %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrZEQs(long hob, size wfac)
{
	printf(" ZEQ %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrZERl2(long arg)
{
	printf(" ZER %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrZERs(long hob, size wfac)
{
	printf(" ZER %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrZERz(void)
{
	printf(" ZER\n");
}

PRIVATE void PrZGEl2(long arg)
{
	printf(" ZGE %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrZGEl4(long arg)
{
	printf(" ZGE %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrZGEs(long hob, size wfac)
{
	printf(" ZGE %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrZGTl2(long arg)
{
	printf(" ZGT %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrZGTl4(long arg)
{
	printf(" ZGT %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrZGTs(long hob, size wfac)
{
	printf(" ZGT %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrZLEl2(long arg)
{
	printf(" ZLE %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrZLEl4(long arg)
{
	printf(" ZLE %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrZLEs(long hob, size wfac)
{
	printf(" ZLE %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrZLTl2(long arg)
{
	printf(" ZLT %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrZLTl4(long arg)
{
	printf(" ZLT %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrZLTs(long hob, size wfac)
{
	printf(" ZLT %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrZNEl2(long arg)
{
	printf(" ZNE %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrZNEl4(long arg)
{
	printf(" ZNE %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrZNEs(long hob, size wfac)
{
	printf(" ZNE %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrZREl2(long arg)
{
	printf(" ZRE %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrZREl4(long arg)
{
	printf(" ZRE %ld\n", Lt_arg_4() * arg);
}

PRIVATE void PrZREs(long hob, size wfac)
{
	printf(" ZRE %ld\n", St_arg(hob) * wfac);
}

PRIVATE void PrZRFl2(long arg)
{
	printf(" ZRF %ld\n", Lt_arg_2() * arg);
}

PRIVATE void PrZRFz(void)
{
	printf(" ZRF\n");
}

PRIVATE void PrZRLm(long arg)
{
	printf(" ZRL %ld\n", arg);
}

PRIVATE void PrZRLn2(long arg)
{
	printf(" ZRL %ld\n", Nt_arg_2() * arg);
}

PRIVATE void PrZRLn4(long arg)
{
	printf(" ZRL %ld\n", Nt_arg_4() * arg);
}

PRIVATE void PrZRLp2(long arg)
{
	printf(" ZRL %ld\n", Pt_arg_2() * arg);
}

PRIVATE void PrZRLp4(long arg)
{
	printf(" ZRL %ld\n", Pt_arg_4() * arg);
}

PRIVATE void PrZRLs(long hob, size wfac)
{
	printf(" ZRL %ld\n", St_arg(hob) * wfac);
}

/* Original code! */

/******** Disassembly of Text Segment ********/

struct ep
{ /* procedure entry points */
	int ep_idf;
	ptr ep_ep;
	size ep_nloc;
};

void disassemble(void)
{
	register ptr low = 0;
	register ptr high = DB;
	register int idf;
	register int cnt;
	struct ep *ep; /* list of entry points */

	/* collect the entry points */
	ep = (struct ep *) Malloc((size) (NProc * sizeof(struct ep)),
			"entry points");
	for (idf = 0; idf < NProc; idf++)
	{
		register struct proc *pr = &proctab[idf];

		ep[idf].ep_idf = idf;
		ep[idf].ep_ep = pr->pr_ep;
		ep[idf].ep_nloc = pr->pr_nloc;
	}

	/* a very naive sorting algorithm */
	for (idf = 0; idf < NProc; idf++)
	{
		register int jdf;

		for (jdf = 0; jdf < NProc; jdf++)
		{
			if ((ep[idf].ep_ep < ep[jdf].ep_ep) != (idf < jdf))
			{
				struct ep p;
				p = ep[idf];
				ep[idf] = ep[jdf];
				ep[jdf] = p;
			}
		}
	}

	TC = low;
	cnt = 0;
	idf = 0;
	while (TC < high)
	{
		if (cnt == 0)
		{
			printf("%lu\n", TC);
		}
		cnt = (cnt + 1) % 10;

		if (idf < NProc && TC >= ep[idf].ep_ep)
		{
			register struct ep *p = &ep[idf];

			printf("P[%d]+%lu:	; %ld %s\n", p->ep_idf, TC - p->ep_ep,
					p->ep_nloc, p->ep_nloc == 1 ? "local" : "locals");
			idf++;
		}

		do_pr_instr(nextTCbyte()); /* advances TC */
	}
}

/* See switch.c */

PRIVATE void do_pr_instr(unsigned int opcode)
{
	switch (opcode)
	{
#include	"PrCases"	/* for the muscle */
	case SECONDARY:
		do_pr_instr(SEC_BASE + nextTCbyte());
		break;
	case TERTIARY:
		do_pr_instr(TERT_BASE + nextTCbyte());
		break;
	default:
		printf(">>>> bad opcode %u at PC = %lu <<<<\n", opcode, TC);
		break;
	}
}

