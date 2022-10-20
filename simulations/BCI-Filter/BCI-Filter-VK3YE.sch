<QucsStudio Schematic 4.3.1>
<Properties>
View=0,0,1090,800,1,0,0
Grid=10,10,1
DataSet=*.dat
DataDisplay=*.dpl
OpenDisplay=1
showFrame=0
FrameText0=Title
FrameText1=Drawn By:
FrameText2=Date:
FrameText3=Revision:
</Properties>
<Symbol>
</Symbol>
<Components>
Pac P1 1 220 340 18 -26 0 0 "1" 1 "50" 1 "0 dBm" 0 "1 GHz" 0 "26.85" 0 "SUBCLICK" 0
GND * 1 220 370 0 0 0 0
GND * 1 400 370 0 0 0 0
GND * 1 540 370 0 0 0 0
GND * 1 680 370 0 0 0 0
Pac P2 1 820 340 18 -26 0 0 "2" 1 "50" 1 "0" 0 "1 GHz" 0 "26.85" 0 "SUBCLICK" 0
GND * 1 820 370 0 0 0 0
.SP SP1 1 220 440 0 67 0 0 "log" 1 "300kHz" 1 "30MHz" 1 "500" 1 "no" 0 "1" 0 "2" 0 "none" 0
C C1 1 330 260 -27 10 0 0 "2.2nF" 1 "0" 0 "" 0 "neutral" 0 "SMD0603" 0
C C4 1 750 260 -27 10 0 0 "2.2nF" 1 "0" 0 "" 0 "neutral" 0 "SMD0603" 0
C C2 1 470 260 -27 10 0 0 "820pF" 1 "0" 0 "" 0 "neutral" 0 "SMD0603" 0
C C3 1 610 260 -27 10 0 0 "820pF" 1 "0" 0 "" 0 "neutral" 0 "SMD0603" 0
L L1 1 400 340 17 -26 0 1 "2.2uH" 1 "0" 0 "" 0 "SELF-WE-PD3S" 0
L L2 1 540 340 17 -26 0 1 "2.2uH" 1 "0" 0 "" 0 "SELF-WE-PD3S" 0
L L3 1 680 340 17 -26 0 1 "2.2uH" 1 "0" 0 "" 0 "SELF-WE-PD3S" 0
</Components>
<Wires>
220 260 220 310 "" 0 0 0 ""
220 260 300 260 "" 0 0 0 ""
400 260 400 310 "" 0 0 0 ""
540 260 540 310 "" 0 0 0 ""
680 260 680 310 "" 0 0 0 ""
400 260 440 260 "" 0 0 0 ""
360 260 400 260 "" 0 0 0 ""
540 260 580 260 "" 0 0 0 ""
500 260 540 260 "" 0 0 0 ""
640 260 680 260 "" 0 0 0 ""
820 260 820 310 "" 0 0 0 ""
780 260 820 260 "" 0 0 0 ""
680 260 720 260 "" 0 0 0 ""
</Wires>
<Diagrams>
<Rect 690 740 360 220 31 #c0c0c0 1 00 1 0 1 1 1 0 1 1 1 0 1 1 315 0 225 "" "" "">
	<Legend 10 -100 0>
	<"dB(S[1,1])" "" #0000ff 0 3 0 0 0 0 "">
	<"dB(S[2,1])" "" #ff0000 0 3 0 0 0 0 "">
</Rect>
</Diagrams>
<Paintings>
Text 380 440 16 #000000 0 high-pass filter, 3MHz cutoff \n 7^{th} order Chebyshev , T-type, \n impedance 50 ohms
</Paintings>
