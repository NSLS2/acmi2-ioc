import matplotlib.pyplot as plt
from matplotlib.widgets import Button, TextBox
from matplotlib.patches import Rectangle
import tkinter as tk
from tkinter import filedialog

def putRect(x,y,l,w,t,a):
    rect = Rectangle((x, y), l, w,
         transform=fig.transFigure,linewidth=t,
         edgecolor=(0,0,0,1), facecolor=(.5,.5,.5,a))
    fig.patches.append(rect)  # or fig.add_artist(rect)

def putText(x,y,l,w,t,text):
    putRect(x,y,l,w,t,0.5)
    ftext = fig.text(x+l/2,y-0.001+w/2,text,ha='center',va='center',
        fontsize=10,fontfamily='monospace')
    return ftext
    
def putLED(x,y,l,w,c):
    # Color: 1=red, else=gray
    color = 'gray'
    if(c==1):
        color='red'
    rect = Rectangle((x, y), l, w,
         transform=fig.transFigure,linewidth=2,
         edgecolor=(0,0,0,1), facecolor=color)
    fig.patches.append(rect)  # or fig.add_artist(rect)
    return rect
    
def LoadFile(event):
    print("Load File!")
    filepath = filedialog.askopenfilename(
    	initialdir="/home/diag/acmi2/PM",title="Select PM File",
    	filetypes=[("Text files", "*.txt"), ("All files", "*.*")])
    if filepath:
        print("Selected:", filepath)
        with open(filepath, 'r') as f:
            line = f.readlines()

            for i in range(0,19):
                line[i] = line[i].strip()
                Dat = [int(l) for l in line[i].split(",")]
                if(len(Dat)==5):
                    if(i==0):
                        T1dat[i].set_text(float(Dat[0])/10000.0)
                        T2dat[i].set_text(float(Dat[1])/10000.0)
                        T3dat[i].set_text(float(Dat[2])/10000.0)
                        BMdat[i].set_text(float(Dat[3])/10000.0)
                        COWdat[i].set_text(float(Dat[4])/10000.0) 
                    else:
                        T1dat[i].set_text(Dat[0])
                        T2dat[i].set_text(Dat[1])
                        T3dat[i].set_text(Dat[2])
                        if(i<9):
                            BMdat[i].set_text(Dat[3])
                        if(i<7):
                            COWdat[i].set_text(Dat[4])
                        if(i==10):
                            print(Dat[4])
                            FliveHex.set_text(hex(Dat[4]))
                            Flive[0].set_facecolor('red' if (Dat[4]&0x01)>0 else 'gray')
                            Flive[1].set_facecolor('red' if (Dat[4]&0x02)>0 else 'gray')
                            Flive[2].set_facecolor('red' if (Dat[4]&0x04)>0 else 'gray')
                            Flive[3].set_facecolor('red' if (Dat[4]&0x08)>0 else 'gray')
                            Flive[4].set_facecolor('red' if (Dat[4]&0x10)>0 else 'gray')
                            Flive[5].set_facecolor('red' if (Dat[4]&0x20)>0 else 'gray')
                            Flive[6].set_facecolor('red' if (Dat[4]&0x40)>0 else 'gray')
                            Flive[7].set_facecolor('red' if (Dat[4]&0x80)>0 else 'gray')
                            Flive[8].set_facecolor('red' if (Dat[4]&0x100)>0 else 'gray')
                            Flive[9].set_facecolor('red' if (Dat[4]&0x200)>0 else 'gray')
                            Flive[10].set_facecolor('red' if (Dat[4]&0x400)>0 else 'gray')
                            Flive[11].set_facecolor('red' if (Dat[4]&0x800)>0 else 'gray')
                            Flive[12].set_facecolor('red' if (Dat[4]&0x1000)>0 else 'gray')
                            Flive[13].set_facecolor('red' if (Dat[4]&0x2000)>0 else 'gray')
                            Flive[14].set_facecolor('red' if (Dat[4]&0x4000)>0 else 'gray')
                            ACCdat[1].set_text(Dat[3])
                        if(i==11):
                            FlatHex.set_text(hex(Dat[4]))
                            Flatch[0].set_facecolor('red' if (Dat[4]&0x01)>0 else 'gray')
                            Flatch[1].set_facecolor('red' if (Dat[4]&0x02)>0 else 'gray')
                            Flatch[2].set_facecolor('red' if (Dat[4]&0x04)>0 else 'gray')
                            Flatch[3].set_facecolor('red' if (Dat[4]&0x08)>0 else 'gray')
                            Flatch[4].set_facecolor('red' if (Dat[4]&0x10)>0 else 'gray')
                            Flatch[5].set_facecolor('red' if (Dat[4]&0x20)>0 else 'gray')
                            Flatch[6].set_facecolor('red' if (Dat[4]&0x40)>0 else 'gray')
                            Flatch[7].set_facecolor('red' if (Dat[4]&0x80)>0 else 'gray')
                            Flatch[8].set_facecolor('red' if (Dat[4]&0x100)>0 else 'gray')
                            Flatch[9].set_facecolor('red' if (Dat[4]&0x200)>0 else 'gray')
                            Flatch[10].set_facecolor('red' if (Dat[4]&0x400)>0 else 'gray')
                            Flatch[11].set_facecolor('red' if (Dat[4]&0x800)>0 else 'gray')
                            Flatch[12].set_facecolor('red' if (Dat[4]&0x1000)>0 else 'gray')
                            Flatch[13].set_facecolor('red' if (Dat[4]&0x2000)>0 else 'gray')
                            Flatch[14].set_facecolor('red' if (Dat[4]&0x4000)>0 else 'gray')
                            ACCdat[2].set_text(Dat[3])
                        if(i==8):
                            CRCeeprom.set_text('0x'+str(format(Dat[4] & 0xFFFFFFFF, '08X')))
                        if(i==9):
                            CRCartix.set_text('0x'+str(format(Dat[4] & 0xFFFFFFFF, '08X')))
                            ACCdat[0].set_text(Dat[3])
                        if(i==13):
                            TS.set_text(Dat[4])
                            ACCdat[4].set_text(Dat[3])
                        if(i==14):
                            TNS.set_text(Dat[4])
                        if(i==15):
                            CNT.set_text(Dat[4])
                        if(i==16):
                            NUM.set_text(Dat[4])
                        if(i==17):
                            DEL.set_text(Dat[4])
            TP1=[]
            TP2=[]
            TP3=[]
            BM=[]
            for i in range(20,112):
                line[i] = line[i].strip()
                Dat = [float(l) for l in line[i].split(",")]
                if(len(Dat)==4):
                    TP1.append(Dat[0])
                    TP2.append(Dat[1])
                    TP3.append(Dat[2])
                    BM.append(Dat[3])        
            COW=[]
            for i in range(114,130):
                line[i] = line[i].strip()
                Dat = [float(l) for l in line[i].split(",")]
                print(Dat)
                if(len(Dat)==8):
                    COW.append(Dat[0])
                    COW.append(Dat[1])
                    COW.append(Dat[2])
                    COW.append(Dat[3])
                    COW.append(Dat[4])
                    COW.append(Dat[5])
                    COW.append(Dat[6])
                    COW.append(Dat[7])
            ACC=[]
            for i in range(132,231):
                line[i] = line[i].strip()
                Dat = [float(l) for l in line[i].split(",")]
                print(Dat)
                if(len(Dat)==10):
                    ACC.append(Dat[0])
                    ACC.append(Dat[1])
                    ACC.append(Dat[2])
                    ACC.append(Dat[3])
                    ACC.append(Dat[4])
                    ACC.append(Dat[5])
                    ACC.append(Dat[6])
                    ACC.append(Dat[7]) 
                    ACC.append(Dat[8])
                    ACC.append(Dat[9]) 
                                   
            ax1.clear()
            ax2.clear()
            ax3.clear()
            ax4.clear()
            ax5.clear()
            ax6.clear()
                    
            ax1.plot(TP1,'-o',markersize=3)
            ax1.set_xlabel("ADC Samples")
            ax1.set_ylabel("ADC Counts")
            ax1.grid(True)
            
            ax2.plot(TP2,'-o',markersize=3)
            ax2.set_xlabel("ADC Samples")
            ax2.set_ylabel("ADC Counts")
            ax2.grid(True)
            
            ax3.plot(TP3,'-o',markersize=3)
            ax3.set_xlabel("ADC Samples")
            ax3.set_ylabel("ADC Counts")
            ax3.grid(True)
            
            ax4.plot(COW,'-o',markersize=3)
            ax4.set_xlabel("ADC Samples")
            ax4.set_ylabel("ADC Counts")
            ax4.grid(True)
            
            ax5.plot(BM,'-o',markersize=3)
            ax5.set_xlabel("ADC Samples")
            ax5.set_ylabel("ADC Counts")
            ax5.grid(True)
            
            ax6.plot(ACC,'-o',markersize=3)
            ax6.set_xlabel("Triggers")
            ax6.set_ylabel("Accumuated Q (nC)")
            ax6.grid(True)

        
root = tk.Tk()
root.withdraw()  # hide the main Tk window

fig = plt.figure(figsize=(12,12))

putRect(0.04,0.7,0.275,0.265,2,0.2)
ax1 = fig.add_axes([0.105, 0.75, 0.20, 0.20])
putText(0.04, 0.97,0.275,0.015,1,"Post Mortem: Test Pulse 1")
putText(0.04,0.68,0.05,0.015,1,"Q(nC)")
putText(0.04,0.66,0.05,0.015,1,"Intg")
putText(0.04,0.64,0.05,0.015,1,"Peak")
putText(0.182,0.68,0.05,0.015,1,"FWHM")
putText(0.182,0.66,0.05,0.015,1,"Indx")
putText(0.182,0.64,0.05,0.015,1,"Base")
putText(0.04,0.62,0.275,0.015,1,"TP1 EEPROM Settings")
putText(0.04,0.60,0.06,0.015,1,"Intg H")
putText(0.04,0.58,0.06,0.015,1,"Intg L")
putText(0.04,0.56,0.06,0.015,1,"Peak H")
putText(0.04,0.54,0.06,0.015,1,"Peak L")
putText(0.182,0.60,0.06,0.015,1,"FWHM H")
putText(0.182,0.58,0.06,0.015,1,"FWHM L")
putText(0.182,0.56,0.06,0.015,1,"Base H")
putText(0.182,0.54,0.06,0.015,1,"Base L")
putText(0.04,0.52,0.06,0.015,1,"PulDly")
putText(0.04,0.50,0.06,0.015,1,"PulWid")
putText(0.182,0.52,0.06,0.015,1,"PosLev")
putText(0.182,0.50,0.06,0.015,1,"NegLev")
putText(0.04,0.48,0.06,0.015,1,"AdcDly")
T1dat = []
T1dat.append(putText(0.093,0.68,0.08,0.015,1,"0.000"))
T1dat.append(putText(0.093,0.66,0.08,0.015,1,"0"))
T1dat.append(putText(0.093,0.64,0.08,0.015,1,"0"))
T1dat.append(putText(0.235,0.68,0.08,0.015,1,"0"))
T1dat.append(putText(0.235,0.66,0.08,0.015,1,"0"))
T1dat.append(putText(0.235,0.64,0.08,0.015,1,"0"))
T1dat.append(putText(0.103,0.60,0.07,0.015,1,"0"))
T1dat.append(putText(0.103,0.58,0.07,0.015,1,"0"))
T1dat.append(putText(0.103,0.56,0.07,0.015,1,"0"))
T1dat.append(putText(0.103,0.54,0.07,0.015,1,"0"))
T1dat.append(putText(0.245,0.60,0.07,0.015,1,"0"))
T1dat.append(putText(0.245,0.58,0.07,0.015,1,"0"))
T1dat.append(putText(0.245,0.56,0.07,0.015,1,"0"))
T1dat.append(putText(0.245,0.54,0.07,0.015,1,"0"))
T1dat.append(putText(0.103,0.52,0.07,0.015,1,"0"))
T1dat.append(putText(0.103,0.50,0.07,0.015,1,"0"))
T1dat.append(putText(0.245,0.52,0.07,0.015,1,"0"))
T1dat.append(putText(0.245,0.50,0.07,0.015,1,"0"))
T1dat.append(putText(0.103,0.48,0.07,0.015,1,"0"))

x = 0.28
ax2 = fig.add_axes([0.105+x, 0.75, 0.20, 0.20])
putRect(0.04+x,0.7,0.275,0.265,2,0.2)
putText(0.04+x, 0.97,0.275,0.015,1,"Post Mortem: Test Pulse 2")
putText(0.04+x,0.68,0.05,0.015,1,"Q(nC)")
putText(0.04+x,0.66,0.05,0.015,1,"Intg")
putText(0.04+x,0.64,0.05,0.015,1,"Peak")
putText(0.182+x,0.68,0.05,0.015,1,"FWHM")
putText(0.182+x,0.66,0.05,0.015,1,"Indx")
putText(0.182+x,0.64,0.05,0.015,1,"Base")
putText(0.04+x,0.62,0.275,0.015,1,"TP2 EEPROM Settings")
putText(0.04+x,0.60,0.06,0.015,1,"Intg H")
putText(0.04+x,0.58,0.06,0.015,1,"Intg L")
putText(0.04+x,0.56,0.06,0.015,1,"Peak H")
putText(0.04+x,0.54,0.06,0.015,1,"Peak L")
putText(0.182+x,0.60,0.06,0.015,1,"FWHM H")
putText(0.182+x,0.58,0.06,0.015,1,"FWHM L")
putText(0.182+x,0.56,0.06,0.015,1,"Base H")
putText(0.182+x,0.54,0.06,0.015,1,"Base L")
putText(0.04+x,0.52,0.06,0.015,1,"PulDly")
putText(0.04+x,0.50,0.06,0.015,1,"PulWid")
putText(0.182+x,0.52,0.06,0.015,1,"PosLev")
putText(0.182+x,0.50,0.06,0.015,1,"NegLev")
putText(0.04+x,0.48,0.06,0.015,1,"AdcDly")
T2dat = []
T2dat.append(putText(0.093+x,0.68,0.08,0.015,1,"0.000"))
T2dat.append(putText(0.093+x,0.66,0.08,0.015,1,"0"))
T2dat.append(putText(0.093+x,0.64,0.08,0.015,1,"0"))
T2dat.append(putText(0.235+x,0.68,0.08,0.015,1,"0"))
T2dat.append(putText(0.235+x,0.66,0.08,0.015,1,"0"))
T2dat.append(putText(0.235+x,0.64,0.08,0.015,1,"0"))
T2dat.append(putText(0.103+x,0.60,0.07,0.015,1,"0"))
T2dat.append(putText(0.103+x,0.58,0.07,0.015,1,"0"))
T2dat.append(putText(0.103+x,0.56,0.07,0.015,1,"0"))
T2dat.append(putText(0.103+x,0.54,0.07,0.015,1,"0"))
T2dat.append(putText(0.245+x,0.60,0.07,0.015,1,"0"))
T2dat.append(putText(0.245+x,0.58,0.07,0.015,1,"0"))
T2dat.append(putText(0.245+x,0.56,0.07,0.015,1,"0"))
T2dat.append(putText(0.245+x,0.54,0.07,0.015,1,"0"))
T2dat.append(putText(0.103+x,0.52,0.07,0.015,1,"0"))
T2dat.append(putText(0.103+x,0.50,0.07,0.015,1,"0"))
T2dat.append(putText(0.245+x,0.52,0.07,0.015,1,"0"))
T2dat.append(putText(0.245+x,0.50,0.07,0.015,1,"0"))
T2dat.append(putText(0.103+x,0.48,0.07,0.015,1,"0"))

x = 0.56
ax3 = fig.add_axes([0.105+x, 0.75, 0.20, 0.20])
putRect(0.04+x,0.7,0.275,0.265,2,0.2)
putText(0.04+x, 0.97,0.275,0.015,1,"Post Mortem: Test Pulse 3")
putText(0.04+x,0.68,0.05,0.015,1,"Q(nC)")
putText(0.04+x,0.66,0.05,0.015,1,"Intg")
putText(0.04+x,0.64,0.05,0.015,1,"Peak")
putText(0.182+x,0.68,0.05,0.015,1,"FWHM")
putText(0.182+x,0.66,0.05,0.015,1,"Indx")
putText(0.182+x,0.64,0.05,0.015,1,"Base")
putText(0.04+x,0.62,0.275,0.015,1,"TP3 EEPROM Settings")
putText(0.04+x,0.60,0.06,0.015,1,"Intg H")
putText(0.04+x,0.58,0.06,0.015,1,"Intg L")
putText(0.04+x,0.56,0.06,0.015,1,"Peak H")
putText(0.04+x,0.54,0.06,0.015,1,"Peak L")
putText(0.182+x,0.60,0.06,0.015,1,"FWHM H")
putText(0.182+x,0.58,0.06,0.015,1,"FWHM L")
putText(0.182+x,0.56,0.06,0.015,1,"Base H")
putText(0.182+x,0.54,0.06,0.015,1,"Base L")
putText(0.04+x,0.52,0.06,0.015,1,"PulDly")
putText(0.04+x,0.50,0.06,0.015,1,"PulWid")
putText(0.182+x,0.52,0.06,0.015,1,"PosLev")
putText(0.182+x,0.50,0.06,0.015,1,"NegLev")
putText(0.04+x,0.48,0.06,0.015,1,"AdcDly")
T3dat = []
T3dat.append(putText(0.093+x,0.68,0.08,0.015,1,"0.000"))
T3dat.append(putText(0.093+x,0.66,0.08,0.015,1,"0"))
T3dat.append(putText(0.093+x,0.64,0.08,0.015,1,"0"))
T3dat.append(putText(0.235+x,0.68,0.08,0.015,1,"0"))
T3dat.append(putText(0.235+x,0.66,0.08,0.015,1,"0"))
T3dat.append(putText(0.235+x,0.64,0.08,0.015,1,"0"))
T3dat.append(putText(0.103+x,0.60,0.07,0.015,1,"0"))
T3dat.append(putText(0.103+x,0.58,0.07,0.015,1,"0"))
T3dat.append(putText(0.103+x,0.56,0.07,0.015,1,"0"))
T3dat.append(putText(0.103+x,0.54,0.07,0.015,1,"0"))
T3dat.append(putText(0.245+x,0.60,0.07,0.015,1,"0"))
T3dat.append(putText(0.245+x,0.58,0.07,0.015,1,"0"))
T3dat.append(putText(0.245+x,0.56,0.07,0.015,1,"0"))
T3dat.append(putText(0.245+x,0.54,0.07,0.015,1,"0"))
T3dat.append(putText(0.103+x,0.52,0.07,0.015,1,"0"))
T3dat.append(putText(0.103+x,0.50,0.07,0.015,1,"0"))
T3dat.append(putText(0.245+x,0.52,0.07,0.015,1,"0"))
T3dat.append(putText(0.245+x,0.50,0.07,0.015,1,"0"))
T3dat.append(putText(0.103+x,0.48,0.07,0.015,1,"0"))

y=0.52
putRect(0.04,0.7-y,0.275,0.265,2,0.2)
ax4 = fig.add_axes([0.105, 0.75-y, 0.20, 0.20])
putText(0.04, 0.97-y,0.275,0.015,1,"Post Mortem: COW")
putText(0.04,0.68-y,0.05,0.015,1,"Q(nC)")
putText(0.04,0.66-y,0.05,0.015,1,"Intg")
putText(0.04,0.64-y,0.05,0.015,1,"Peak")
putText(0.182,0.68-y,0.05,0.015,1,"FWHM")
putText(0.182,0.66-y,0.05,0.015,1,"Indx")
putText(0.182,0.64-y,0.05,0.015,1,"Base")
putText(0.04,0.62-y,0.275,0.015,1,"COW EEPROM Settings")
putText(0.04,0.60-y,0.15,0.015,1,"Peak Threshold")

COWdat = []
COWdat.append(putText(0.093,0.68-y,0.08,0.015,1,"0.000"))
COWdat.append(putText(0.093,0.66-y,0.08,0.015,1,"0"))
COWdat.append(putText(0.093,0.64-y,0.08,0.015,1,"0"))
COWdat.append(putText(0.235,0.68-y,0.08,0.015,1,"0"))
COWdat.append(putText(0.235,0.66-y,0.08,0.015,1,"0"))
COWdat.append(putText(0.235,0.64-y,0.08,0.015,1,"0"))
COWdat.append(putText(0.193,0.60-y,0.122,0.015,1,"0"))

x = 0.28
y = 0.52
ax5 = fig.add_axes([0.105+x, 0.75-y, 0.20, 0.20])
putRect(0.04+x,0.7-y,0.275,0.265,2,0.2)
putText(0.04+x, 0.97-y,0.275,0.015,1,"Post Mortem: Beam Pulse")
putText(0.04+x,0.68-y,0.05,0.015,1,"Q(nC)")
putText(0.04+x,0.66-y,0.05,0.015,1,"Intg")
putText(0.04+x,0.64-y,0.05,0.015,1,"Peak")
putText(0.182+x,0.68-y,0.05,0.015,1,"FWHM")
putText(0.182+x,0.66-y,0.05,0.015,1,"Indx")
putText(0.182+x,0.64-y,0.05,0.015,1,"Base")
putText(0.04+x,0.62-y,0.275,0.015,1,"Beam EEPROM Settings")
putText(0.04+x,0.60-y,0.06,0.015,1,"Base H")
putText(0.04+x,0.58-y,0.06,0.015,1,"Base L")
putText(0.182+x,0.60-y,0.06,0.015,1,"Intg H")
BMdat = []
BMdat.append(putText(0.093+x,0.68-y,0.08,0.015,1,"0.000"))
BMdat.append(putText(0.093+x,0.66-y,0.08,0.015,1,"0"))
BMdat.append(putText(0.093+x,0.64-y,0.08,0.015,1,"0"))
BMdat.append(putText(0.235+x,0.68-y,0.08,0.015,1,"0"))
BMdat.append(putText(0.235+x,0.66-y,0.08,0.015,1,"0"))
BMdat.append(putText(0.235+x,0.64-y,0.08,0.015,1,"0"))
BMdat.append(putText(0.245+x,0.60-y,0.07,0.015,1,"0"))
BMdat.append(putText(0.103+x,0.60-y,0.07,0.015,1,"0"))
BMdat.append(putText(0.103+x,0.58-y,0.07,0.015,1,"0"))

x = 0.56
y = 0.52
ax6 = fig.add_axes([0.105+x, 0.75-y, 0.20, 0.20])
putRect(0.04+x,0.7-y,0.275,0.265,2,0.2)
putText(0.04+x, 0.97-y,0.275,0.015,1,"Post Mortem: Accumulator")
putText(0.04+x,0.68-y,0.15,0.015,1,"Accumulator Q (nC)")
putText(0.04+x,0.66-y,0.15,0.015,1,"Accum Integral Sum")
putText(0.04+x,0.64-y,0.275,0.015,1,"Accumulator EEPROM Settings")
putText(0.04+x,0.62-y,0.15,0.015,1,"Accum Integral Max")
putText(0.04+x,0.60-y,0.15,0.015,1,"Accum Length")
ACCdat = []
ACCdat.append(putText(0.193+x,0.68-y,0.122,0.015,1,"0"))
ACCdat.append(putText(0.193+x,0.66-y,0.122,0.015,1,"0"))
ACCdat.append(putText(0.193+x,0.62-y,0.122,0.015,1,"0"))
ACCdat.append(putText(0.193+x,0.60-y,0.122,0.015,1,"0"))

putText(0.88, 0.97,0.1,0.015,1,"Faults")
putText(0.88, 0.95,0.048,0.015,1,"Live")
putText(0.932, 0.95,0.048,0.015,1,"Lat")
Flive = []
Flatch = []
Flive.append(putLED(0.88,0.93,0.015,0.015,0))
putText(0.9, 0.93,0.06,0.015,1,"BM QH")
Flatch.append(putLED(0.965,0.93,0.015,0.015,0))
Flive.append(putLED(0.88,0.91,0.015,0.015,0))
putText(0.9, 0.91,0.06,0.015,1,"ACC H")
Flatch.append(putLED(0.965,0.91,0.015,0.015,0))
Flive.append(putLED(0.88,0.89,0.015,0.015,0))
putText(0.9, 0.89,0.06,0.015,1,"BM BL")
Flatch.append(putLED(0.965,0.89,0.015,0.015,0))
Flive.append(putLED(0.88,0.87,0.015,0.015,0))
putText(0.9, 0.87,0.06,0.015,1,"TP1")
Flatch.append(putLED(0.965,0.87,0.015,0.015,0))
Flive.append(putLED(0.88,0.85,0.015,0.015,0))
putText(0.9, 0.85,0.06,0.015,1,"TP2")
Flatch.append(putLED(0.965,0.85,0.015,0.015,0))
Flive.append(putLED(0.88,0.83,0.015,0.015,0))
putText(0.9, 0.83,0.06,0.015,1,"TP3")
Flatch.append(putLED(0.965,0.83,0.015,0.015,0))
Flive.append(putLED(0.88,0.81,0.015,0.015,0))
putText(0.9, 0.81,0.06,0.015,1,"CRC")
Flatch.append(putLED(0.965,0.81,0.015,0.015,0))
Flive.append(putLED(0.88,0.79,0.015,0.015,0))
putText(0.9, 0.79,0.06,0.015,1,"COW")
Flatch.append(putLED(0.965,0.79,0.015,0.015,0))
Flive.append(putLED(0.88,0.77,0.015,0.015,0))
putText(0.9, 0.77,0.06,0.015,1,"PWR")
Flatch.append(putLED(0.965,0.77,0.015,0.015,0))
Flive.append(putLED(0.88,0.75,0.015,0.015,0))
putText(0.9, 0.75,0.06,0.015,1,"ADC")
Flatch.append(putLED(0.965,0.75,0.015,0.015,0))
Flive.append(putLED(0.88,0.73,0.015,0.015,0))
putText(0.9, 0.73,0.06,0.015,1,"KEY")
Flatch.append(putLED(0.965,0.73,0.015,0.015,0))
Flive.append(putLED(0.88,0.71,0.015,0.015,0))
putText(0.9, 0.71,0.06,0.015,1,"FORCE")
Flatch.append(putLED(0.965,0.71,0.015,0.015,0))
Flive.append(putLED(0.88,0.69,0.015,0.015,0))
putText(0.9, 0.69,0.06,0.015,1,"START")
Flatch.append(putLED(0.965,0.69,0.015,0.015,0))
Flive.append(putLED(0.88,0.67,0.015,0.015,0))
putText(0.9, 0.67,0.06,0.015,1,"CLOCK")
Flatch.append(putLED(0.965,0.67,0.015,0.015,0))
Flive.append(putLED(0.88,0.65,0.015,0.015,0))
putText(0.9, 0.65,0.06,0.015,1,"TRIG")
Flatch.append(putLED(0.965,0.65,0.015,0.015,0))
putText(0.88, 0.63,0.04,0.015,1,"Live")
putText(0.88, 0.61,0.04,0.015,1,"Lat")
FliveHex = putText(0.922, 0.63,0.06,0.015,1,"0x00")
FlatHex = putText(0.922, 0.61,0.06,0.015,1,"0x00")

putText(0.88, 0.57,0.1,0.015,1,"CRC EEPROM")
CRCeeprom = putText(0.88, 0.55,0.1,0.015,1,"0x00")
putText(0.88, 0.51,0.1,0.015,1,"CRC ARTIX")
CRCartix = putText(0.88, 0.49,0.1,0.015,1,"0x00")

putText(0.88, 0.45,0.1,0.015,1,"Timestamp")
putText(0.88, 0.43,0.025,0.015,1,"Sec")
putText(0.88, 0.41,0.025,0.015,1,"NS")
putText(0.88, 0.39,0.025,0.015,1,"Cnt")
TS = putText(0.907, 0.43,0.072,0.015,1,"0")
TNS = putText(0.907, 0.41,0.072,0.015,1,"0")
CNT = putText(0.907, 0.39,0.072,0.015,1,"0")

putText(0.88, 0.35,0.1,0.015,1,"EVR Trigger")
putText(0.88, 0.33,0.025,0.015,1,"Num")
putText(0.88, 0.31,0.025,0.015,1,"Del")
NUM = putText(0.907, 0.33,0.072,0.015,1,"0")
DEL = putText(0.907, 0.31,0.072,0.015,1,"0")

Load_ax = fig.add_axes([0.7, 0.02, 0.1, 0.03])
Load = Button(Load_ax, 'Load File')
Load.label.set_fontsize(12)
                  
Load.on_clicked(LoadFile)
plt.show()
