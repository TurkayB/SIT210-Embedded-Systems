from tkinter import *
import tkinter.font
from gpiozero import LED
import RPi.GPIO
RPi.GPIO.setmode(RPi.GPIO.BCM)

led_red = LED(14)
led_blue = LED(15)
led_green = LED(18)

win = Tk()
win.title("LED Switches")
myFont = tkinter.font.Font(family = 'Helvetica', size = 12, weight = "bold")

def toggleRed():
        if led_red.is_lit:
                led_red.off()
                redButton["text"] = "Turn red lED on"
        else:
                led_blue.off()
                blueButton["text"] = "Turn blue LED on"
                led_green.off()
                greenButton["text"] = "Turn green LED on"
                led_red.on()
                redButton["text"] = "Turn red LED off"

def toggleBlue():
        if led_blue.is_lit:
                led_blue.off()
                blueButton["text"] = "Turn blue LED on"
        else:
                led_red.off()
                redButton["text"] = "Turn red LED on"
                led_green.off()
                greenButton["text"] = "Turn green LED on"
                led_blue.on()
                blueButton["text"] = "Turn blue LED off"

def toggleGreen():
        if led_green.is_lit:
                led_green.off()
                greenButton["text"] = "Turn green LED on"
        else:
                led_red.off()
                redButton["text"] = "Turn red LED on"
                led_blue.off()
                blueButton["text"] = "Turn blue LED on"
                led_green.on()
                greenButton["text"] = "Turn green LED off"
def close():
        RPi.GPIO.cleanup()
        win.destroy()

redButton = Button(win, text = 'Turn red LED on', font = myFont, command = toggleRed, bg = 'bisque2', height = 1, width = 24)
redButton.grid(row=0,column=1)

blueButton = Button(win, text = 'Turn blue LED on', font = myFont, command = toggleBlue, bg = 'bisque2', height = 1, width = 24)
blueButton.grid(row=1,column=1)

greenButton = Button(win, text = 'Turn green LED on', font = myFont, command = toggleGreen, bg = 'bisque2', height = 1, width = 24)
greenButton.grid(row=2,column=1)

exitButton = Button(win, text = 'Exit', font = myFont, command = close, bg = 'red', height = 1, width = 6)
exitButton.grid(row=3,column=1)

win.protocol("WM_DELETE_WINDOW", close)

win.mainloop()