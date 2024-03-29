CAPTURING = True

import numpy as np
import cv2

import signal
import sys

from pyfirmata import Arduino, util
from time import sleep
import time

from netifaces import ifaddresses

import RPi.GPIO as GPIO

import serial
import json

BUTTON_GPIO = 24

def get_my_ip():
    try:
        wlan0 = ifaddresses('wlan0')
        if 2 in wlan0:
            return wlan0[2][0]['addr']
        else:
            return None
    except ValueError:
        return None

ser = serial.Serial("/dev/ttyAMA0", 115200, timeout = 0.5)

def get_temp_param():
    ser.reset_input_buffer()
    s = ser.readline()
    if len(s) == 0:
        return None
    try:
        return json.loads(s)
    except Exception:
        return None

HEIGHT = 600
WIDTH = 1024

blobs = 0
bpm = None
N = 0.1
prev_blob_time = None

DEBUG = True

CAPTURE_TIMEOUT = 300

def blob_callback(channel):
    global blobs
    global bpm
    global prev_blob_time

    if prev_blob_time == None:
        prev_blob_time = time.time()
        return

    blobs += 1
    interval = time.time() - prev_blob_time
    next_bpm = 60. / interval
    prev_blob_time = time.time()

    if bpm == None:
        bpm = next_bpm
    else:
        bpm = bpm * (1. - N) + next_bpm * N

    if DEBUG:
        print(
            "get blob. Time: %f, interval: %f, next_bpm: %d, bpm: %d" %
            (prev_blob_time, interval, next_bpm, bpm)
        )

frame_counter = 0

def process_image(src, debug=True, update_contour=True):
    global frame_counter
    frame_counter += 1

    out_img = cv2.resize(src, (WIDTH, HEIGHT))

    font = cv2.FONT_HERSHEY_SIMPLEX
    my_ip = get_my_ip()
    if my_ip is not None:
        cv2.putText(
            out_img, "ip: %s" % my_ip,
            (10,15),
            font, .5, (250,250,250), 1, cv2.LINE_AA
        )

    cv2.putText(
        out_img, "blob counter: %d" % blobs,
        (80,80),
        font, 2.5, (0,0,255), 8, cv2.LINE_AA
    )

    if(bpm != None):
        cv2.putText(
            out_img, "%d BPM" % bpm,
            (80,150),
            font, 2.5, (0,0,255), 8, cv2.LINE_AA
        )

    temp_param = get_temp_param()
    if temp_param is not None:
        cv2.putText(
            out_img, "target temp: %s" % temp_param["target_temp"],
            (80,200),
            font, 1.5, (0,0,250), 3, cv2.LINE_AA
        )
        cv2.putText(
            out_img, "current temp: %s" % temp_param["temp"],
            (80,240),
            font, 1.5, (0,0,250), 3, cv2.LINE_AA
        )

    if frame_counter % 20 == 0 and temp_param is not None:
        print(temp_param, "bpm:", bpm, ", blobs:", blobs)

    return out_img

def do_exit():
    if cap:
        cap.release()
    
    cv2.destroyAllWindows()
    sys.exit(0)

def signal_handler(sig, frame):
    print('You pressed Ctrl+C!')
    do_exit()

def camera_thread(cap):
    print("BPM started")

    no_frame_counter = 0
    MAX_FRAME_SKIPPED = 100
    frame_counter = 0

    led_state = 0

    prev_time = time.time()
    capture_idx = 0

    while(True):
        if led_state == 1:
            led_state = 0
        else:
            led_state = 1

        # board.digital[13].write(led_state)
        
        # Capture frame-by-frame
        ret, frame = cap.read()
        frame_counter += 1

        if not ret:
            no_frame_counter += 1
            if no_frame_counter > MAX_FRAME_SKIPPED:
                print("no more frames, exit")
                do_exit()
            
            cv2.waitKey(1)
            continue
        else:
            no_frame_counter = 0

        # Our operations on the frame come here
        out_img = process_image(frame)

        if CAPTURING and time.time() - prev_time > CAPTURE_TIMEOUT:
            print("capuring:", capture_idx)
            capture_idx += 1
            prev_time = time.time()
            cv2.imwrite("timelapse_" + str(int(time.time())) + ".jpg", out_img)

        # Display the resulting frame
        cv2.imshow('frame', out_img)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            pass

def load_image(image_name):
    print("load image", image_name)
    img = cv2.imread(image_name)
    # cv2.imshow(image_name, img)
    out_img = process_image(img)
    cv2.imshow(image_name, out_img)
    cv2.waitKey(0)

signal.signal(signal.SIGINT, signal_handler)

GPIO.setmode(GPIO.BCM)
GPIO.setup(BUTTON_GPIO, GPIO.IN, pull_up_down=GPIO.PUD_OFF)
GPIO.add_event_detect(BUTTON_GPIO, GPIO.RISING, callback=blob_callback, bouncetime=100)

if len(sys.argv) > 1:
    load_image(sys.argv[1])
else:
    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, WIDTH)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, HEIGHT)

    camera_thread(cap)

do_exit()
