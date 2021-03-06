v 20130925 2
C 40000 40000 0 0 0 title-B.sym
C 44100 47400 1 0 0 SA612.sym
{
T 44700 49300 5 10 1 1 0 6 1
refdes=U201
T 44400 54000 5 10 0 0 0 0 1
device=SA612A
T 44400 54200 5 10 0 0 0 0 1
footprint=DIP8
}
C 48100 47700 1 90 0 capacitor-1.sym
{
T 47400 47900 5 10 0 0 90 0 1
device=CAPACITOR
T 47800 48400 5 10 1 1 180 0 1
refdes=C205
T 47200 47900 5 10 0 0 90 0 1
symversion=0.1
T 48000 47900 5 10 1 1 0 0 1
value=10n
}
C 47000 46400 1 0 0 capacitor-1.sym
{
T 47200 47100 5 10 0 0 0 0 1
device=CAPACITOR
T 47000 46800 5 10 1 1 0 0 1
refdes=C203
T 47200 47300 5 10 0 0 0 0 1
symversion=0.1
T 47600 46300 5 10 1 1 0 0 1
value=1n
}
C 49100 47700 1 90 0 capacitor-1.sym
{
T 48400 47900 5 10 0 0 90 0 1
device=CAPACITOR
T 48800 48400 5 10 1 1 180 0 1
refdes=C206
T 48200 47900 5 10 0 0 90 0 1
symversion=0.1
T 49000 47900 5 10 1 1 0 0 1
value=0.1u
}
C 43100 48700 1 180 0 capacitor-1.sym
{
T 42900 48000 5 10 0 0 180 0 1
device=CAPACITOR
T 42100 48700 5 10 1 1 0 0 1
refdes=C201
T 42900 47800 5 10 0 0 180 0 1
symversion=0.1
T 42800 48200 5 10 1 1 0 0 1
value=0.1u
}
C 44100 46800 1 90 0 capacitor-1.sym
{
T 43400 47000 5 10 0 0 90 0 1
device=CAPACITOR
T 43800 47600 5 10 1 1 180 0 1
refdes=C202
T 43200 47000 5 10 0 0 90 0 1
symversion=0.1
T 44000 46900 5 10 1 1 0 0 1
value=1n
}
C 51600 44900 1 270 0 capacitor-1.sym
{
T 52300 44700 5 10 0 0 270 0 1
device=CAPACITOR
T 51200 44600 5 10 1 1 0 0 1
refdes=C212
T 52500 44700 5 10 0 0 270 0 1
symversion=0.1
T 51900 44100 5 10 1 1 0 0 1
value=1u
}
C 47600 44400 1 90 0 capacitor-1.sym
{
T 46900 44600 5 10 0 0 90 0 1
device=CAPACITOR
T 47100 45100 5 10 1 1 180 0 1
refdes=C208
T 46700 44600 5 10 0 0 90 0 1
symversion=0.1
T 47600 44400 5 10 1 1 0 0 1
value=0.1u
}
C 49900 48800 1 0 0 coil-1.sym
{
T 50100 49200 5 10 0 0 0 0 1
device=COIL
T 49900 49000 5 10 1 1 0 0 1
refdes=L201
T 50100 49400 5 10 0 0 0 0 1
symversion=0.1
T 50500 48600 5 10 1 1 0 0 1
value=MPZ1608D101B
}
C 50000 45300 1 0 0 coil-1.sym
{
T 50200 45700 5 10 0 0 0 0 1
device=COIL
T 50000 45500 5 10 1 1 0 0 1
refdes=L204
T 50200 45900 5 10 0 0 0 0 1
symversion=0.1
T 50500 45000 5 10 1 1 0 0 1
value=BLM21RK102
}
C 44000 45900 1 90 0 resistor-1.sym
{
T 43600 46200 5 10 0 0 90 0 1
device=RESISTOR
T 43700 46700 5 10 1 1 180 0 1
refdes=R201
T 44000 46000 5 10 1 1 0 0 1
value=51
}
N 44100 47900 43900 47900 4
C 42000 47300 1 0 0 gnd-1.sym
C 47800 47300 1 0 0 gnd-1.sym
C 52300 47300 1 0 0 gnd-1.sym
C 43100 47300 1 0 0 gnd-1.sym
C 47300 42400 1 0 0 gnd-1.sym
N 43900 47700 43900 47900 4
C 48500 44400 1 90 0 capacitor-1.sym
{
T 47800 44600 5 10 0 0 90 0 1
device=CAPACITOR
T 48200 45100 5 10 1 1 180 0 1
refdes=C209
T 47600 44600 5 10 0 0 90 0 1
symversion=0.1
T 48500 44400 5 10 1 1 0 0 1
value=1n
}
C 48200 43500 1 0 0 capacitor-1.sym
{
T 48400 44200 5 10 0 0 0 0 1
device=CAPACITOR
T 48000 43900 5 10 1 1 0 0 1
refdes=C204
T 48400 44400 5 10 0 0 0 0 1
symversion=0.1
T 48900 43400 5 10 1 1 0 0 1
value=22p
}
N 45700 43700 48200 43700 4
N 46400 48200 46700 48200 4
N 46000 45300 50000 45300 4
N 43900 45900 45100 45900 4
C 41100 49200 1 0 0 input-1.sym
{
T 41100 49500 5 10 0 0 0 0 1
device=INPUT
T 40000 49200 5 10 1 1 0 0 1
net=ToSA612_P
}
C 45100 45800 1 0 0 output-1.sym
{
T 45200 46100 5 10 0 0 0 0 1
device=OUTPUT
T 46000 45800 5 10 1 1 0 0 1
net=MIX_OUT
}
C 41100 48400 1 0 0 input-1.sym
{
T 41100 48700 5 10 0 0 0 0 1
device=INPUT
T 40100 48400 5 10 1 1 0 0 1
net=ToSA612_N
}
C 52200 49300 1 0 0 vcc_1.sym
N 51000 45300 53000 45300 4
C 54600 44100 1 90 0 capacitor-1.sym
{
T 53900 44300 5 10 0 0 90 0 1
device=CAPACITOR
T 54300 44800 5 10 1 1 180 0 1
refdes=C213
T 53700 44300 5 10 0 0 90 0 1
symversion=0.1
T 54500 44300 5 10 1 1 0 0 1
value=1u
}
C 55400 44100 1 90 0 resistor-1.sym
{
T 55000 44400 5 10 0 0 90 0 1
device=RESISTOR
T 55200 44900 5 10 1 1 180 0 1
refdes=R203
T 55400 44200 5 10 1 1 0 0 1
value=47K
}
C 54300 42500 1 0 0 gnd-1.sym
C 55100 45800 1 0 0 vcc_1.sym
N 55300 45800 55300 45000 4
T 52200 45400 9 10 1 0 0 0 1
3.3V
C 51700 42400 1 0 0 gnd-1.sym
N 51800 44000 51800 42700 4
C 52700 43700 1 0 0 NJM2865.sym
{
T 52800 44395 5 10 1 1 0 0 1
model-name=NJM2865
T 53600 45395 5 10 1 1 0 0 1
refdes=U203
}
C 44300 43500 1 0 0 Si501_MEMS.sym
{
T 44400 44800 5 10 1 1 0 0 1
refdes=U202
T 44400 44600 5 10 1 1 0 0 1
model=Si501
T 44900 44600 5 10 1 1 0 0 1
value=100MHz
}
C 43900 42400 1 0 0 gnd-1.sym
N 47400 42700 47400 44400 4
N 48300 44400 48300 44300 4
N 48300 44300 47400 44300 4
N 53300 43700 53300 43000 4
N 53300 43000 54400 43000 4
N 55300 43300 55300 44100 4
N 53000 43700 53000 43300 4
N 53000 43300 55300 43300 4
C 50000 47700 1 90 0 capacitor-1.sym
{
T 49300 47900 5 10 0 0 90 0 1
device=CAPACITOR
T 49700 48400 5 10 1 1 180 0 1
refdes=C207
T 49100 47900 5 10 0 0 90 0 1
symversion=0.1
T 49900 47900 5 10 1 1 0 0 1
value=1u
}
N 47900 48600 47900 48800 4
N 46400 48800 49900 48800 4
C 42100 49100 1 0 0 capacitor-1.sym
{
T 42300 49800 5 10 0 0 0 0 1
device=CAPACITOR
T 42100 49600 5 10 1 1 0 0 1
refdes=C211
T 42300 50000 5 10 0 0 0 0 1
symversion=0.1
T 42700 49000 5 10 1 1 0 0 1
value=10n
}
C 42100 49900 1 0 0 capacitor-1.sym
{
T 42300 50600 5 10 0 0 0 0 1
device=CAPACITOR
T 42100 50400 5 10 1 1 0 0 1
refdes=C210
T 42300 50800 5 10 0 0 0 0 1
symversion=0.1
T 42700 49900 5 10 1 1 0 0 1
value=1u
}
N 51800 44900 51800 45300 4
N 53600 45300 55300 45300 4
N 54400 45000 54400 45300 4
N 54400 44100 54400 42800 4
N 49800 47700 47900 47700 4
N 47900 47700 47900 47600 4
N 48900 48600 48900 48800 4
N 49800 48600 49800 48800 4
C 52600 47800 1 90 0 capacitor-1.sym
{
T 51900 48000 5 10 0 0 90 0 1
device=CAPACITOR
T 52200 48500 5 10 1 1 180 0 1
refdes=C214
T 51700 48000 5 10 0 0 90 0 1
symversion=0.1
T 52600 47800 5 10 1 1 0 0 1
value=10n
}
N 41900 49300 41900 50100 4
N 43300 50100 43300 49300 4
N 43000 49300 44100 49300 4
N 44100 49300 44100 48800 4
N 44100 48200 43200 48200 4
N 43200 48200 43200 47600 4
N 44100 48500 43100 48500 4
N 41900 48500 42200 48500 4
N 42100 48500 42100 47600 4
N 46000 45300 46000 44300 4
N 46000 44300 45700 44300 4
N 44300 43700 44000 43700 4
N 44000 43700 44000 42700 4
N 47900 46600 49500 46600 4
N 49500 46600 49500 43700 4
N 49500 43700 49100 43700 4
N 42100 50100 41900 50100 4
N 42100 49300 41900 49300 4
N 43300 50100 43000 50100 4
N 46700 48200 46700 46600 4
N 46700 46600 47000 46600 4
N 52400 48700 52400 49300 4
N 52400 48800 50900 48800 4
N 52400 47800 52400 47600 4
