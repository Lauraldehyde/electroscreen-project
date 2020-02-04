# Author: Laura Moody
# Version: v1
# Date: 2019.07.18

from tkinter import *
from tkinter import font
from tkinter import ttk
from tkinter import messagebox
import datetime
import time
from pyscreenshot import grab
from PIL import Image
global version
global comName
version = "v1.0"
comName = "USB-SERIAL CH340"
import serial
from serial import tools
from serial.tools import list_ports
import glob
import sys
import struct
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np
import atexit
import os

global rawData 
rawData = open("ghost.txt", "w+")

#serial classes
class esConnection:
    def __init__(self, master):
        self.portDescriptions()
        self.reading = ''
        self.array = []
        self.xArray = []
        self.y1Array = []
        self.y2Array = []
        self.junk = ''
        self.line = ''
        self.decodeLine = ''
    
    def checkConnection(self, master):
        global comName
        self.comport = self.findPort(comName)
        self.ser = serial.Serial()
        if self.connectPort(self.ser, self.comport[0]):
            return(TRUE)
        else:
            return(FALSE)

    def findPort(self, name):
        self.ports = list( serial.tools.list_ports.comports())
        self.usbFound = 0
        self.devices = []
        for port in self.ports:
            if port.description.startswith(name):
                print(port.device)
                self.devices.append(port.device)
                self.usbFound += 1
            else:
                continue
        if self.usbFound == 0:
            print("No USB matching that description was found")
            self.devices.append(FALSE)
            return(self.devices)
        else:
            print("There were ", self.usbFound, " devices found.")
            return(self.devices)
    #method to return comport list and descriptions
    def portDescriptions(self):
        self.ports = list( serial.tools.list_ports.comports())

        self.resultPorts = []
        self.descriptions = []

        for port in self.ports:
            self.result = port.device
            self.resultPorts.append(self.result)
            self.description = port.description
            self.descriptions.append(self.description)
        
        self.portNum = len(self.resultPorts)

        for x in range(self.portNum):
            print("COMPORT NAME: ", self.resultPorts[x])
            print("DESCRIPTION: ", self.descriptions[x])
            print("--------------------------")

    def connectPort(self,ser, portName):
        try:
            self.ser.baudrate = 9600
            self.ser.port = portName
            self.ser.timeout = 2
            self.ser.open()
            return True
        except(ValueError, serial.SerialException):
            return False

    def readPort(self,ser):
        self.line = ser.readline()
        #print(str(line))
        return(self.line)

    def writePort(self,ser, data):
        ser.write(data)

    def inputParas(self,ser,high, low, scan, repeats, go):
        ser.write(str(high).encode("ascii"))
        print(self.readPort(ser))
        ser.write(str(low).encode("ascii"))
        print(self.readPort(ser))
        ser.write(str(scan).encode("ascii"))
        print(self.readPort(ser))
        ser.write(str(repeats).encode("ascii"))
        print(self.readPort(ser))
        ser.write(str(go).encode("ascii"))
        print(self.readPort(ser))

    def calcLines(self,high, low, repeats):
        self.digiHigh = self.convVol(high)
        self.digiLow = self.convVol(low)
        self.lineNo = (2 * (self.digiLow - self.digiHigh + 1) * repeats)
        return self.lineNo


    def convVol(self,vol):
        self.digital = (-1 * vol) + 1000
        self.res = 2000.0/255.0
        self.digital2 = (self.digital/self.res)
        self.digital3 = round(self.digital2)
        return self.digital3

    def isNumComma(self,char):
        if char == "0" or 0:
            return "0"
        elif char == "1" or 1:
            return "1"
        elif char == "2" or 2:
            return "2"
        elif char == "3" or 3:
            return "3"
        elif char == "4" or 4:
            return "4"
        elif char == "5" or 5:
            return "5"
        elif char == "6" or 6:
            return "6"
        elif char == "7" or 7:
            return "7"
        elif char == "8" or 8:
            return "8"
        elif char == "9" or 9:
            return "9"
        elif char == ",":
            return ","
        else:
            return (False)

    def figFrame(self, master):
        self.fig = serialGraph(master, self)

    def figInit(self):
        self.fig.subPlt.plot(self.xArray, self.y1Array)
        return self.fig.line,
    
    def csvShowData(self, iterate):
        if iterate < (int(self.reading)):
            self.line = self.readPort(self.ser)
            self.decodeLine = self.line.decode("ascii")
            self.array.append(self.decodeLine)
            self.split = self.decodeLine.split(",")
            try:
                self.xInt = int(self.split[0])
                self.yInt = int(self.split[1])
                self.xArray.append(self.xInt)
                self.y1Array.append(self.yInt)
                print("Raw data: " + self.array[iterate])
                global rawData
                rawData.write(str(self.xInt) + "," + str(self.yInt) + "\n")
                #convert print function to csv file saving when needed
                self.pltData = self.fig.subPlt
                #self.fig.subPlt.plot(self.xArray, self.y1Array)
                self.pltData.clear()
                global filename
                self.fig.subPlt.set_title(filename + " voltammogram")
                self.fig.subPlt.set_xlabel("V")
                self.fig.subPlt.set_ylabel("uA")
                self.pltData.plot(self.xArray, self.y1Array)
                #could use this to update progress bar rather than using indterminate one
                return 
            except:
                rawData.csvFile.close()
                print("Reading has finsihed.")
                self.showResult(True, 2)
                self.ser.close()
                global path
                global root
                root.update()
                self.imgSave = tkWinGrab(root, path, filename)
                self.imgSave.saveImg()
        else:
            rawData.csvFile.close()
            print("Reading has finished")
            self.ser.close()
            #create code to finish serial 
            self.showResult(True, 2)
            root.update()
            self.imgSave = tkWinGrab(root, path, filename)
            self.imgSave.saveImg()

    def progressFrame(self, master):
        self.pframe = ttk.Frame(master)
        self.label = ttk.Label(self.pframe, text = "Results")
        self.label.config(font = ("Arial", 14, "bold"))
        self.progress = ttk.Progressbar(self.pframe, orient = HORIZONTAL, length =200)
        self.progress.config(mode = 'indeterminate')
        self.label.grid(row = 0, column = 0, padx=5, pady=5)
        self.progress.grid(row =2, column = 0, columnspan = 2, padx=5, pady=5)
        self.progress.start()

    def showResult(self, tfbool, conc):
        self.progress.grid_forget()
        self.resultLabel = ttk.Label(self.pframe, font = ('Arial'))
        self.resultLabel.grid(row =1, column =0)
        if tfbool:
            self.resultLabel.config(text = "POSITIVE", foreground = 'green')
        else:
            self.resultLabel.config(text = "NEGATIVE", foreground = 'red')
        self.concLabel = ttk.Label(self.pframe, text = ("Mesothelin concentration: " + str(conc) + " nM"))
        self.concLabel.grid(row=2, column =0)

class startScreen:
    def __init__(self, master):
        self.frame = ttk.Frame(master)
        self.frame.grid(row=0, column = 0)
        self.frame.config(padding = (10,10,10,10))
        self.startImg = PhotoImage(file = "startscreen.gif")
        self.label = ttk.Label(self.frame, text = "", image = self.startImg)
        self.label.pack()
        self.button = ttk.Button(self.frame, text = "Connect to device", style = 'Blue.TButton')
        self.button.config(command = lambda: self.connect(master))
        for Widget in self.frame.pack_slaves():
            Widget.pack_configure(expand = True, padx = 10, pady = 10)
        self.button.pack(pady = 20)

    def connect(self, master):
            if arduino.checkConnection(master):
                self.frame.grid_forget() #remove start screen from master 
                global pEntry
                pEntry.frame.grid(row = 0, column = 0) #add patient entry screen to master
                global feet
                feet.changeInstructions(master, "Entry instructions", "1. Click on the details button, a pop-up will appear.\n2. Enter the TrialNo and patient initials on the pop-up.\n3. Click submit details button", "ins-img.gif" )#change footer instructions
                messagebox.showinfo("Connected", "ES device was found")
            else:
                messagebox.showerror("Connection error", "The computer could not find device, please check physical connections and re-try device discovery.")

class footer:
    def __init__(self, master):
        self.frame = ttk.Frame(master)
        self.frame.grid(row =1, column =0, sticky = W + E)
        self.frame.rowconfigure(0, weight =1)
        self.frame.columnconfigure(0, weight =1)
        self.frame.columnconfigure(1, weight =1)
        self.logo = PhotoImage(file = 'UoP-cropped.gif')
        self.label = ttk.Label(self.frame, image = self.logo, text = "Developed at University of Portsmouth. \n2017-2020 Laura Moody \nVersion 1.0", compound = 'left')
        self.label.img = self.logo
        self.label.config(font = ("Arial", 10))
        self.ask = ttk.Label(self.frame, text = "?", font =('Arial', 54, 'bold'), foreground = '#0070C0')
        self.ask.bind('<1>', lambda e: self.seeInstructions(master))
        self.ask.bind('<Enter>', lambda e: self.mouseover())
        self.ask.bind('<Leave>', lambda e: self.mouseleave())
        self.label.grid(row =0, column =0, sticky = SW, ipadx = 10)
        self.ask.grid(row =0, column = 1, sticky = SE)
        for Widget in self.frame.grid_slaves():
            Widget.grid_configure(ipadx = 5, ipady = 5, padx = 10, pady =10)
        self.ask.grid_configure(pady = 0, ipady = 0)
        self.title = "Electroscreen set-up instructions"
        self.lblText = "1. Place your well into the enclave as shown below - do not add anything yet.\n2. Connect your device to the computer via the USB cable.\n3. Press the connect button"
        self.img = 'ins-img.gif'
    
    def changeInstructions(self, master, title, lblText, img):
        self.title = title
        self.lblText = lblText
        self.img = img
    
    def seeInstructions(self, master):
        setupIns(master, self.title, self.lblText, self.img)
    
    def mouseover(self):
        self.ask.config(foreground = '#FFC000')

    def mouseleave(self):
        self.ask.config(foreground = '#0070C0')

class patientEntry:
    def __init__(self, master):
        self.frame = ttk.Frame(master)
        self.label1 = ttk.Label(self.frame, text = "Device connection sucessful")
        self.label1.config(foreground = '#0070C0', font = ('Arial', 32))
        self.label2 = ttk.Label(self.frame, text = "RFID/ experiment parameters recieved.")
        self.label2.config(foreground = 'black', font = ('Arial', 18))
        self.button = ttk.Button(self.frame, text = "Enter sample details", command = lambda: self.patientInput(master), style = "Blue.TButton")
        self.label1.pack()
        self.label2.pack()
        self.button.pack()
        for widget in self.frame.pack_slaves():
            widget.pack_configure(padx = 20, pady =20)
        

    def patientInput(self, master):
        self.window = Toplevel(master)
        self.window.title("Sample details input")
        self.window.wm_iconbitmap('favicon.ico')
        self.window.resizable(False, False)
        self.window.config(background = 'white')
        self.label3 = ttk.Label(self.window, text = "Please enter the sample details below and click submit when finished. These details are used to generate your file name so it is important to input them correctly")
        self.label3.config(wraplength = 300, justify = LEFT,)
        self.tLabel = ttk.Label(self.window, text = "Trial number: ")
        self.iLabel = ttk.Label(self.window, text = "Initials: ")
        now = datetime.datetime.now()
        global date
        global entryTime
        date = now.strftime("%d") + "-" + now.strftime("%m") + "-" + now.strftime("%Y")
        entryTime = now.strftime("%H") + "-" + now.strftime("%M") + "-" + now.strftime("%S")
        self.dtLabel = ttk.Label(self.window, text = ("Date: " + date + " Time: " + entryTime))
        self.tEntry = ttk.Entry(self.window, width = 15)
        self.iEntry = ttk.Entry(self.window, width = 15)
        self.tVar = StringVar()
        self.iVar = StringVar()
        #create buttons
        self.submit = ttk.Button(self.window, text = "Submit details", command = lambda: self.btnCommand(master), style = 'Detail.TButton')
        self.clear = ttk.Button(self.window, text="Clear form", command = lambda: self.clearBtn(master), style = 'Detail.TButton')
        #put in window by grid manager
        self.label3.grid(row = 0, column = 0, columnspan =2)
        self.tLabel.grid(row = 2, column = 0, sticky = E)
        self.tEntry.grid(row = 2, column = 1, sticky = W)
        self.iLabel.grid(row =3, column = 0, sticky = E)
        self.iEntry.grid(row =3, column = 1, sticky = W)
        self.dtLabel.grid(row =1, column = 0, columnspan =2)
        self.submit.grid(row =4, column = 0, sticky = E)
        self.clear.grid(row = 4, column = 1, sticky = W)
        for Widget in self.window.grid_slaves():
            Widget.grid_configure(padx = 10, pady = 10)
        center(self.window)

    
    def btnCommand(self, master):
        #get values in Entry box
        self.tVar = self.tEntry.get()
        self.iVar = self.iEntry.get()
        print("Trial No: " + self.tVar)
        print("Initials: " + self.iVar)
        global trialNo
        trialNo = self.tVar
        global initials
        initials = self.iVar
        self.clearBtn(master)
        global filename
        global date
        global entryTime
        filename = trialNo + "_" + initials + "_" + date + "_" + entryTime
        print("Filename: " + filename)
        if self.tVar.isdigit():
            self.window.state('withdrawn')
            messagebox.showinfo("Check your details", "The next page will show the sample and testing equipment details. Please check these are correct before proceeding.")
            global pCheck
            pCheck = detailCheck(master)
            self.frame.grid_forget()
            pCheck.frame.grid(row=0, column =0)
            global feet
            feet.changeInstructions(master, "Check your details", "1. Check the sample details entered are correct and the test parameters meet your expectations. \n2. If the sample details need to be amended use the 're-enter details button.\n3. If the testing details are wrong please find the approriate test, insert into device, and restart the program.\n4. If all details are correct, click the begin testing button.", "ins-img.gif")
        else:
            messagebox.showerror("Input error","Trial number shows non-digit characters, cannot submit. Please re-try using digits only.")
    
    def clearBtn(self, master):
        self.tEntry.delete(0, END)
        self.iEntry.delete(0, END)

class detailCheck:
    def __init__(self, master):
        self.frame = ttk.Frame(master)
        self.title1 = ttk.Label(self.frame, text = "Testing details")
        self.title2 = ttk.Label(self.frame, text = "Sample details")
        self.title1.config(font = ('Arial', 18, 'bold'), foreground = "#0070C0")      
        self.title2.config(font = ('Arial', 18, 'bold'), foreground ='#0070C0')
        

        #device details
        global batch
        self.label0_0 = ttk.Label(self.frame, text = ("Batch No: " + batch), style = 'Detail.TLabel')
        global biomarker
        biomarker = "mesothelin"
        global molarity
        molarity = "2 nM"
        self.label0_1 = ttk.Label(self.frame, text = ("Testing for " + biomarker + ". Cut-off " + molarity + "."), style = 'Detail.TLabel')
        global cvHigh
        global cvLow
        self.label0_2 = ttk.Label(self.frame, text = ("CV " + str(cvLow) + " to +" + str(cvHigh) + " mV"), style = 'Detail.TLabel')
        global cvRate
        global scanRepeat
        self.label0_3 = ttk.Label(self.frame, text = (str(cvRate) + " mV/s, " + str(scanRepeat) + " scans"), style = 'Detail.TLabel')
        global analysisType
        analysisType = "Max height analysis"
        self.label0_4 = ttk.Label(self.frame, text = analysisType, style = 'Detail.TLabel')

        #sample details
        global trialNo
        self.label1_0 = ttk.Label(self.frame, text =("Trial No: " + trialNo), style = 'Detail.TLabel')
        global initials
        self.label1_1 = ttk.Label(self.frame, text = ("Initials: " + initials), style = 'Detail.TLabel')
        global date
        self.label1_2 = ttk.Label(self.frame, text = ("Date: " + date), style = 'Detail.TLabel')
        global entryTime
        self.label1_3 = ttk.Label(self.frame, text = ("Time of data entry: " + entryTime), style = 'Detail.TLabel')

        #create buttons and load into grid
        global arduino
        self.testBtn = ttk.Button(self.frame, text = "Begin testing", style = "Blue.TButton", command = lambda: self.tCommand(master,arduino))
        self.re_enterBtn = ttk.Button(self.frame, text = "Re-enter details", command = lambda: self.rCommand(master), style = "Small.TButton")
        self.title1.grid(row = 0, column =0, sticky = W)
        self.title2.grid(row = 0, column =1, sticky = W)
        self.label0_0.grid(row = 1, column = 0, sticky = W)
        self.label0_1.grid(row =2, column = 0, sticky = W)
        self.label0_2.grid(row =3, column = 0, sticky = W)
        self.label0_3.grid(row =4, column = 0, sticky = W)
        self.label0_4.grid(row =5, column = 0, sticky = W)
        self.label1_0.grid(row = 1, column = 1, sticky = W)
        self.label1_1.grid(row =2, column = 1, sticky = W)
        self.label1_2.grid(row =3, column = 1, sticky = W)
        self.label1_3.grid(row =4, column = 1, sticky = W)
        self.testBtn.grid(row =6, column = 0, columnspan =2)
        self.re_enterBtn.grid(row = 7, column = 0, columnspan =2)
        for widget in self.frame.grid_slaves():
            widget.grid_configure(padx= 5, pady = 5)
        self.testBtn.grid_configure(ipadx = 10, ipady = 5, padx =20, pady=20)


    def tCommand(self, master, comm):
        #disable buttons
        self.testBtn.config(state = 'disabled', cursor = "wait")
        self.re_enterBtn.config(state = 'disabled', cursor = "wait")
        
        #write test button command
        comm.readPort(comm.ser)
        self.go =1
        global cvHigh
        global cvLow
        global scanRepeat
        global cvRate
        global filename
        global rawData
        rawData = csvstorage(filename)
        comm.reading = comm.calcLines(cvHigh, cvLow, scanRepeat)        
        print("calcLines has output: ", comm.reading)
        comm.inputParas(comm.ser,cvHigh,cvLow,cvRate,scanRepeat,self.go)
        comm.junk = comm.readPort(comm.ser)
        print(comm.junk)
        comm.junk = comm.readPort(comm.ser)
        print(comm.junk)

        #remove detail check from window
        self.frame.grid_forget()
        self.testBtn.config(cursor = "")
        self.re_enterBtn.config(cursor = "")
        #call initialisation of serial graph
        self.testFrame = ttk.Frame(master)
        comm.figFrame(self.testFrame)
        self.detailList = sampleLabel(self.testFrame)
        comm.progressFrame(self.testFrame)
        #put serial graph and test details in the frame
        self.detailList.frame.grid(row=0, column=0)
        comm.pframe.grid(row=1, column=0)
        comm.fig.frame.grid(row = 0, column = 1, rowspan = 2)
        self.testFrame.grid(row=0, column=0)

    def rCommand(self, master):
        #write re-entry command
        print("Trigger re-entry process")
        pEntry.patientInput(master)

class setupIns:
    def __init__(self, master, title, lblText, img):
        self.window = Toplevel(master)
        self.window.title(title)
        self.window.wm_iconbitmap('favicon.ico')
        self.frame = ttk.Frame(self.window)
        self.frame.pack()
        self.label = ttk.Label(self.frame, text = lblText) #"Set-up instructions will be added here."
        self.image = PhotoImage(file = img) #'Set-up.gif'
        self.label.pack()
        self.picLbl = ttk.Label(self.frame, text = "", image = self.image, compound = LEFT)
        self.picLbl.image = self.image #needed to keep a reference to the image so it isn't garbage collected and shows as transparent
        self.picLbl.pack()
        center(self.window)

class fakeRFIDread:
    def __init__(self):
        global batch
        batch = "01"
        global biomarker
        biomarker = "mesothelin"
        global molarity
        molarity = "2 nM"
        global cvHigh
        cvHigh = 600
        global cvLow
        cvLow = -200
        global cvRate
        cvRate = 400
        global scanRepeat
        scanRepeat = 1
        global analysisType
        analysisType = "max height analysis"

class csvstorage:
    def __init__(self, name):
        #create a folder for patient number and date if it does not already exist
        global trialNo
        global date
        global path
        path = ("Sample results/" + trialNo + "/" + date)
        if not os.path.exists(path):
            os.makedirs(path)
            print("Directory created")
        #open a file according to file name in the specified directory to hold raw data
        self.csvFile = open((path + "/" + name + ".csv"), "w+")
    
    def write(self, message):
        self.csvFile.write(message)

class serialGraph:
    def __init__(self, master,comm):
        self.frame = ttk.Frame(master)
        self.fig = plt.Figure()
        self.subPlt = self.fig.add_subplot(111)
        global filename
        self.subPlt.set_title(filename + " voltammogram")
        self.subPlt.set_xlabel("V")
        self.subPlt.set_ylabel("uA")
        self.line = self.subPlt.plot(comm.xArray, comm.y1Array)
        self.canvas = FigureCanvasTkAgg(self.fig, master = self.frame)
        self.canvas.get_tk_widget().pack(fill = 'both', expand =1)
        self.ani = animation.FuncAnimation(self.fig, comm.csvShowData, np.arange(0, (int(comm.reading + 1))), init_func=comm.figInit, repeat = False)

class sampleLabel:
    def __init__(self, master):
        self.frame = ttk.Frame(master, )
        self.sampleTitle = ttk.Label(self.frame, text = "Sample Details", font = ('Arial'), foreground = '#0070C0')
        global trialNo
        self.numberLabel = ttk.Label(self.frame, text = ("Trial No: " + trialNo))
        global initials
        self.initialsLabel = ttk.Label(self.frame, text = ("Initials: " + initials))
        global date
        self.dateLabel = ttk.Label(self.frame, text = ("Date: " + date))
        global entryTime
        self.timeLabel= ttk.Label(self.frame, text = ("Time of data entry: " + entryTime))

        self.testTitle = ttk.Label(self.frame, text = "Test Details", font = ('Arial'), foreground = '#0070C0')
        global batch
        self.batchLabel = ttk.Label(self.frame, text = ("Batch: " + str(batch)))
        global biomarker
        global molarity
        self.biomarkerLabel = ttk.Label(self.frame, text = ("Testing for " + biomarker + ". Cut-off: " + molarity))
        global cvLow
        global cvHigh
        self.methodLabel = ttk.Label(self.frame, text = ("CV from " + str(cvLow) + " to " + str(cvHigh) + " mV"))
        global cvRate
        global scanRepeat
        self.scanLabel = ttk.Label(self.frame, text = ("Rate of " + str(cvRate) + ", repeated " + str(scanRepeat) + " times."))
        global analysisType
        self.anaTypeLabel = ttk.Label(self.frame, text = analysisType)

        self.sampleTitle.grid(row = 0, column = 0)
        self.numberLabel.grid(row =1, column = 0)
        self.initialsLabel.grid(row=2, column =0)
        self.dateLabel.grid(row=3, column =0)
        self.timeLabel.grid(row=4, column=0)
        self.testTitle.grid(row=6, column =0)
        self.batchLabel.grid(row=7, column =0)
        self.biomarkerLabel.grid(row=8, column=0)
        self.methodLabel.grid(row=9, column=0)
        self.scanLabel.grid(row=10, column=0)
        self.anaTypeLabel.grid(row=11, column=0)
        
        for Widget in self.frame.grid_slaves():
            Widget.grid_configure(padx = 5, pady =5)

class tkWinGrab:
    def __init__(self, win, dir, filename):
        #save for other method
        self.dir = dir
        #get window/frame co-ordinates
        self.xPos = win.winfo_rootx()
        self.yPos = win.winfo_rooty()
        #get window height and width
        self.winHeight = win.winfo_height()
        self.winWidth = win.winfo_width()
        #use pyscreenshot to get an image grab of just the window
        self.img = grab(bbox=(self.xPos, self.yPos, (self.winWidth + self.xPos), (self.winHeight + self.yPos)))
        #self.img = grab() #whole screen

    def saveImg(self):
        #save the image generated as a png in the 
        self.filePath = self.dir + "/" + filename + ".png"
        print(self.filePath)
        self.img.save(self.filePath)

def center(win):
    win.update_idletasks()
    width = win.winfo_width()
    frm_width = win.winfo_rootx() - win.winfo_x()
    win_width = width + 2 * frm_width
    height = win.winfo_height()
    titlebar_height = win.winfo_rooty() - win.winfo_y()
    win_height = height + titlebar_height + frm_width
    x = win.winfo_screenwidth() // 2 - win_width // 2
    y = win.winfo_screenheight() // 2 - win_height // 2
    win.geometry('{}x{}+{}+{}'.format(width, height, x, y))
    win.deiconify()

def main():
    #important global variables
    global trialNo
    global initials
    global date
    global entryTime
    global filename
    global modeChoice
    global path
    fakeRFIDread()

    #GUI stuff
    global root
    root = Tk()  
    modeChoice = IntVar(root, value= 1)
    global screen_width
    screen_width = root.winfo_screenwidth()
    global screen_height
    screen_height = root.winfo_screenheight()
    widthx = (screen_width/2) - 500
    heighty = (screen_height/2) - 300
    root.geometry('1000x600' + '+%d+%d' % (widthx, heighty))
    root.title("Electroscreen.meso v1.0")
    root.config(background = 'white')
    root.wm_iconbitmap('favicon.ico')
    root.rowconfigure(0, weight =1)
    root.rowconfigure(1, weight =0)
    root.columnconfigure(0, weight =1)

    #Adding menubar
    menubar = Menu(root, tearoff = FALSE)
    root.config(menu=menubar)
    about = Menu(menubar, tearoff = FALSE)
    help_ = Menu(menubar, tearoff = FALSE)
    mode = Menu(about, tearoff = FALSE)
    menubar.add_cascade(menu = about, label = "About")
    menubar.add_cascade(menu = help_, label = "Help")
    about.add_command(label = "Version", command = lambda: messagebox.showinfo("Version", ("This is Electroscreen " + version + ".")))
    about.add_cascade(menu = mode, label = "Mode")
    mode.add_radiobutton(label = "Clinical trial", variable = modeChoice, value =1, command = lambda: messagebox.showinfo("Mode info","This radiobutton will select clinical trial mode when other modes are available."))
    mode.add_radiobutton(label = "Point of care", variable = modeChoice, value =0)
    mode.entryconfig(1, state = DISABLED)
    help_.add_command(label = "User manual", command = lambda: messagebox.showinfo("Manual info","This menu item will open the user manual once finalised"))
    help_.add_command(label = "In program", command = lambda: messagebox.showinfo("In program help", "To access quick step specific help, please click on the '?' displayed in the bottom right corner of the program."))

    global style
    style = ttk.Style()
    style.theme_use('clam')
    style.configure('Blue.TButton', background = '#0070C0', font = ('Arial', 24, 'bold'), foreground = 'white', padding = 10, borderwidth = 5, highlightbackground = '#0070C0', bordercolor = '#0070C0', lightcolor = '#0070C0', focuscolor = '#0070C0', darkcolor = '#0070C0')
    style.map('Blue.TButton', background = [('active', '#FFC000')], bordercolor = [('active', '#FFC000')], lightcolor = [('active', '#FFC000')], darkcolor = [('active', '#FFC000')], focuscolor = [('active', '#FFC000')], highlightbackground = [('active', '#FFC000')])
    style.configure('TFrame', background = 'white', border = 'white')
    style.configure('TLabel', background = 'white')
    style.configure('Blue.TButton.label', background = '#0070C0')
    style.configure('Small.TButton', background = '#FFC000', font = ('Arial', 14), padding = 2, highlightbackground = '#FFC000', bordercolor = '#FFC000', lightcolor = '#FCC000', focuscolor = '#FCC000', darkcolor = '#FCC000')
    style.map('Small.TButton', background = [('active', '#ff4d4d')], highlightbackground = [('active', '#ff4d4d')], bordercolor = [('active', '#ff4d4d')], lightcolor = [('active', '#ff4d4d')], darkcolor = [('active', '#ff4d4d')], focuscolor = [('active', '#ff4d4d')])
    style.configure('Detail.TButton', background = '#0070C0', font = ('Arial', 10), foreground = 'white', highlightbackground = '#0070C0', bordercolor = '#0070C0', lightcolor = '#0070C0', focuscolor = '#0070C0', darkcolor = '#0070C0')
    style.map('Detail.TButton', background = [('active', '#FFC000')], bordercolor = [('active', '#FFC000')], lightcolor = [('active', '#FFC000')], darkcolor = [('active', '#FFC000')], focuscolor = [('active', '#FFC000')], highlightbackground = [('active', '#FFC000')])
    style.configure('Detail.TLabel', foreground = 'black', font = ('Arial', 18), padding =5)
    style.configure('TLabel', font = ('Arial', 10))
    global start
    start = startScreen(root)
    global feet
    feet = footer(root)
    global pEntry
    pEntry = patientEntry(root)
    global pCheck

    #serial communication
    global arduino
    arduino = esConnection(root)
    root.mainloop()

main()


