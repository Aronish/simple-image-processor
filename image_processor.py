from image_processor import * # C++ library with bindings
from PIL import ImageTk, Image as PImage
import tkinter

class Viewer:
    def __init__(self, requested_window_height: int, file_path) -> None:
        self.original_image = PImage.open(file_path)

        aspect_ratio = self.original_image.width / self.original_image.height
        self.height = requested_window_height
        self.width = int(aspect_ratio * self.height)
        self.original_image = self.original_image.resize((self.width, self.height))

        self.org_photo_img = ImageTk.PhotoImage(self.original_image)

    def getModifiedPhotoImg(self, kernel_type):
        # Use C++ class to get a modified image back
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
    root = tkinter.Tk()
    root.title("Image Processor")
    root.resizable(False, False)
    root.lift()
    root.attributes("-topmost", True)
    root.attributes("-topmost", False)

    viewer = Viewer(400, file_path)
    canvas = tkinter.Canvas(root, width=viewer.width * 2, height=viewer.height) # Canvas with space for two images side-by-side

    kernel_choice = tkinter.IntVar()
    def switch_kernel():
        type_key = list(KernelType.__members__)[kernel_choice.get()]
        canvas.itemconfig(convertible_canvas_img, image=viewer.getModifiedPhotoImg(KernelType.__members__[type_key]))

    radio_buttons = []
    for i in range(len(KernelType.__members__)):
        radio_button = tkinter.Radiobutton(root, text=list(KernelType.__members__.keys())[i], variable=kernel_choice, value=i, command=switch_kernel).grid(row=0, column=i)
        radio_buttons.append(radio_button)

    # Display original image
    canvas.create_image(0, 0, anchor=tkinter.NW, image=viewer.org_photo_img)
    # Display modified image
    convertible_canvas_img = canvas.create_image(viewer.width, 0, anchor=tkinter.NW, image=viewer.getModifiedPhotoImg(KernelType.NOTHING))
    canvas.grid(row=1, column=0, columnspan=len(radio_buttons))

    root.mainloop()

if __name__ == "__main__":
    main()