NAME

    DummyFS - The experimental virtual filesystem built on hard drive image.

SYNOPSIS

    DummyFS [IMAGE_FILE]

DESCRIPTION

    DummyFS is an experimental virtual filesystem built on an image file.

    DummyFS is derived from the experimental the Operating System UNIXv6++,
    built by the faculties of Tongji University.

    Open the shell with the IMAGE_FILE initialized by the utility Initialize.

SHELL COMMANDS

    load src dest
        Load file src from the local hard drive and save it in the
        absolute path dest in DummyFS.

    save src dest
        Save the file with the absolute path src in DummyFS to dest in
        the local hard drive.

    ls path
        Listing the directory of the absolute path.

    unlink path
        Remove a file with the absolute path.

    mkdir path
        Make an directory with the absolute path.

    rmdir path
        Remove an empty directory with the absolute path.

    shutdown
        Close and exit the DummyFS shell safely.

AUTHORS

    Archie Lu
    chienius@outlook.com

    Tongji University Computer Science Department Faculty
    http://cs.tongji.edu.cn/
