# Sorter
This program sorts the files in your downloads folder based on file extension.

It uses a list of locations listed in the text file file_and_path.txt

If there is not a configuration stored for an extension in file_and_path.txt it requests one and automatically writes it. 

# Linux only
Not tested or designed for windows or mac. The c++ library filesystem was used to make extending the logic easier however, additiional logic will needed to make the program compatible with windows.

# Not fully tested
I did some rudimentary testing with a single folder structure.

However, I have not written tests yet. 