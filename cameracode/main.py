# Smart Letter Detection Version 2
import sensor, image, time, math

from pyb import LED,Pin

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE)
sensor.set_framesize(sensor.QCIF)
sensor.skip_frames(60)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

ImageX = 176
ImageY = 144

sensor.set_windowing((0, 0, ImageX, ImageY))

high_threshold = (0, 100)
thresholds = (0, 120)

centX = ImageX / 2
centY = ImageY / 2

global best_match
LED(1).off()
LED(2).off()
LED(3).off()

while(True):
    img = sensor.snapshot()

    img.binary([high_threshold])
    img.dilate(2)
    img.binary([high_threshold])

    most_middle = 10000
    match_found = False
    Detectedletter = 0
    leftline=0
    rightline=0
    middleline=0

    for blob in img.find_blobs([thresholds], pixels_threshold=40, area_threshold=50, merge=False):

        # touch side break
        if blob.x() == 0 or blob.y() == 0 or blob.y() + blob.h() >= ImageY - 1 or blob.x() + blob.w() >= ImageX - 1:
            continue

        distance = math.floor( math.sqrt( math.pow((blob.cx() - centX),2 ) + math.pow((blob.cy() - centY),2)))

        if distance < most_middle and blob.pixels() < 3000 and blob.pixels() > 10:
            most_middle = distance
            best_match = blob
            match_found = True

    if match_found:

        MidY = best_match.cy()
        TopY = best_match.y()
        WH = best_match.w()
        HH = best_match.h()
        BotY = best_match.y() + HH
        BotX = best_match.x() + WH
        TopX = best_match.x()


        for Y in range(TopY,BotY,2):
            if img.get_pixel(TopX+int(WH/6), Y) == 0:
                leftline += 1
        leftline=leftline/HH

        for Y in range(TopY,BotY,2):
            if img.get_pixel(TopX+int(WH*5/6), Y) == 0:
                rightline += 1
        rightline=rightline/HH

        for X in range(TopX,BotX,2):
            if img.get_pixel(X, MidY) == 0:
                middleline += 1
        middleline=middleline/WH

        # Detectedletter = int((leftline + middleline + rightline)*1000 - 1000)
        # img.draw_rectangle((TopX,TopY,BotX-TopX,BotY-TopY),color = (150,150,150))
        # img.draw_line((TopX,MidY,BotX,MidY),color = (150,150,150))
        # img.draw_line((TopX+int((BotX - TopX)/6),TopY,TopX+int((BotX - TopX)/6),BotY),color = (150,150,150))
        # img.draw_line((TopX+int((BotX - TopX)*5/6),TopY,TopX+int((BotX - TopX)*5/6),BotY),color = (150,150,150))
        # lastWhite = True

    #debug
    #print("left " + str(int(leftline*100)) + "   middle " + str(int(middleline*100)) + "   right " + str(int(rightline*100)))

    if leftline*100 > 42 and rightline*100 > 42:
        if middleline*100 > 40:
            print("H")
            LED(1).on()
            LED(2).off()
            LED(3).off()
        else:
            print("U")
            LED(1).off()
            LED(2).on()
            LED(3).off()
    elif middleline*100 > 30:
        print("S")
        LED(1).off()
        LED(2).off()
        LED(3).on()
    else:
        LED(1).off()
        LED(2).off()
        LED(3).off()

    match_found = False
