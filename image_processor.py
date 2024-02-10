import gaussian_blur
from PIL import ImageTk, Image as PImage
import tkinter
import sys

def main():
    # Create a simple window
    width = 1200
    height = 600
    root = tkinter.Tk()
    root.resizable(False, False)
    canvas = tkinter.Canvas(root, width=width, height=height)
    
    # Display original image
    original_image = PImage.open("ian_mckellen_gandalf_4k_lotr.jpg").resize((int(width / 2), height))
    org_photo_img = ImageTk.PhotoImage(original_image)
    canvas.create_image(0, 0, anchor=tkinter.NW, image=org_photo_img)

    # Get the blurred image and display it to the right
    blurred = gaussian_blur.BlurredImage(original_image.width, original_image.height, list(original_image.tobytes()))
    blurred_photo_img = ImageTk.PhotoImage(image=PImage.frombytes(data=bytes(blurred.blur()), mode="RGB", size=original_image.size))
    canvas.create_image(int(width / 2), 0, anchor=tkinter.NW, image=blurred_photo_img)
    canvas.pack()

    root.mainloop()

if __name__ == "__main__":
    main()