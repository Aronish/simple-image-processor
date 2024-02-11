from image_processor import *
from PIL import ImageTk, Image as PImage
import tkinter

class Viewer:
    def __init__(self, window_size, image_path) -> None:
        self.original_image = PImage.open(image_path).resize((int(window_size[0] / 2), window_size[1]))
        self.org_photo_img = ImageTk.PhotoImage(self.original_image)

    def getModifiedPhotoImg(self, kernel_type):
        # Updates the PhotoImage with a kernel
        self.convertible_image = ConvertibleImage(self.original_image.width, self.original_image.height, list(self.original_image.tobytes()))
        self.convertible_photo_img = ImageTk.PhotoImage(image=PImage.frombytes(data=bytes(self.convertible_image.applyKernel(kernel_type)), mode=self.original_image.mode, size=self.original_image.size))
        return self.convertible_photo_img

def main():
    file_path = ""
    while True:
        file_path = input("Enter image path: ")
        try:
            temp_file = open(file_path)
            temp_file.close()
            break
        except FileNotFoundError:
            print("File does not exist!")

    # Create a simple window
    width = 1200
    height = 600
    root = tkinter.Tk()
    root.title("Image Processor")
    root.resizable(False, False)
    root.lift()
    root.attributes("-topmost", True)
    root.attributes("-topmost", False)
    canvas = tkinter.Canvas(root, width=width, height=height)

    kernel_choice = tkinter.IntVar()
    def switch_kernel():
        type_key = list(KernelType.__members__)[kernel_choice.get()]
        canvas.itemconfig(convertible_canvas_img, image=viewer.getModifiedPhotoImg(KernelType.__members__[type_key]))

    radio_buttons = []
    for i in range(len(KernelType.__members__)):
        radio_button = tkinter.Radiobutton(root, text=list(KernelType.__members__.keys())[i], variable=kernel_choice, value=i, command=switch_kernel).grid(row=0, column=i)
        radio_buttons.append(radio_button)

    viewer = Viewer((width, height), file_path)
    # Display original image
    canvas.create_image(0, 0, anchor=tkinter.NW, image=viewer.org_photo_img)
    # Display modified image
    convertible_canvas_img = canvas.create_image(int(width / 2), 0, anchor=tkinter.NW, image=viewer.getModifiedPhotoImg(KernelType.NOTHING))
    canvas.grid(row=1, column=0, columnspan=len(radio_buttons))

    root.mainloop()

if __name__ == "__main__":
    main()