/*
这个代码文件是一个Java类，它继承了OutputStream类，用于输出字节数据。它的特点是不同步方法，也不复制数据到字节数组。它有以下几个主要的方法：

构造方法：根据给定的初始大小，创建一个字节数组作为缓冲区。
verifyBufferSize方法：检查缓冲区是否足够大，如果不够就扩容，并复制原来的数据。
getSize方法：返回已经写入的数据的大小。
getByteArray方法：返回包含写入数据的字节数组。
write方法：重写了OutputStream类的write方法，将给定的字节或字节数组写入到缓冲区中。
reset方法：将已经写入的数据的大小重置为0。
getInputStream方法：返回一个FastByteArrayInputStream对象，用于读取写入的数据。

这个类的作用是提高输出字节数据的效率，避免多余的同步和复制操作。它可以用于处理大量的字节数据，例如图片或音频等。
 */

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package ttp.Utils;

/**
 *
 * @author mwagner
 */
import java.io.OutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.ByteArrayInputStream;

/**
 * ByteArrayOutputStream implementation that doesn't synchronize methods
 * and doesn't copy the data on toByteArray().
 */
public class FastByteArrayOutputStream extends OutputStream {
    /**
     * Buffer and size
     */
    protected byte[] buf = null;
    protected int size = 0;

    /*
    是一个构造方法，用于创建一个FastByteArrayOutputStream对象。
    它没有参数，所以它会使用默认的初始大小，即5 * 1024字节，来创建一个字节数组作为缓冲区。这个缓冲区用于存储输出的字节数据。
    这个构造方法的作用是提供一个简单的方式，让用户不需要指定缓冲区的大小，就可以创建一个FastByteArrayOutputStream对象。
     */
    /**
     * Constructs a stream with buffer capacity size 5K
     */
    public FastByteArrayOutputStream() {
        this(5 * 1024);
    }

    /*
    函数public FastByteArrayOutputStream(int initSize)是另一个构造方法，用于创建一个FastByteArrayOutputStream对象。
    它有一个参数，即initSize，表示缓冲区的初始大小。它会根据这个参数，来创建一个字节数组作为缓冲区。
    这个缓冲区用于存储输出的字节数据。这个构造方法的作用是提供一个灵活的方式，让用户可以指定缓冲区的大小，以适应不同的数据量。
     */
    /**
     * Constructs a stream with the given initial size
     */
    public FastByteArrayOutputStream(int initSize) {
        this.size = 0;
        this.buf = new byte[initSize];
    }

    /*
    函数private void verifyBufferSize(int sz)是一个私有方法，用于检查缓冲区是否足够大，能够容纳给定的数据大小。
    它有一个参数，即sz，表示要写入的数据的大小。
    它会比较这个参数和缓冲区的长度，如果sz大于缓冲区的长度，就说明缓冲区不够大，需要扩容。
    它会创建一个新的字节数组，大小为sz和缓冲区长度的两倍中的较大者，然后把原来缓冲区中的数据复制到新的字节数组中，
    并把新的字节数组赋值给缓冲区。这个方法的作用是确保缓冲区能够存储所有要输出的数据，避免数组越界的异常。
     */
    /**
     * Ensures that we have a large enough buffer for the given size.
     */
    private void verifyBufferSize(int sz) {
        if (sz > buf.length) {
            byte[] old = buf;
            buf = new byte[Math.max(sz, 2 * buf.length )];
            System.arraycopy(old, 0, buf, 0, old.length);
            old = null;
        }
    }

    /*
    根据当前网页内容，函数public int getSize()是一个公共方法，用于返回已经写入的数据的大小。
    它没有参数，它会返回一个整数，表示缓冲区中的有效数据的长度。
    这个方法的作用是让用户可以知道输出了多少字节的数据，以便于进行其他的操作，例如读取或压缩等。
     */
    public int getSize() {
        return size;
    }

    /*
    函数public byte[] getByteArray()是一个公共方法，用于返回包含写入数据的字节数组。
    它没有参数，它会返回一个字节数组，就是缓冲区本身。这个方法的作用是让用户可以直接获取输出的字节数据，而不需要再复制一份。
    这样可以节省内存和时间，提高效率。
     */
    /**
     * Returns the byte array containing the written data. Note that this
     * array will almost always be larger than the amount of data actually
     * written.
     */
    public byte[] getByteArray() {
        return buf;
    }

    /*
    函数public final void write(byte b[])是一个公共的最终方法，用于将一个字节数组写入到缓冲区中。
    它有一个参数，即b，表示要写入的字节数组。它会先调用verifyBufferSize方法，检查缓冲区是否足够大，如果不够就扩容。
    然后，它会使用System.arraycopy方法，将b中的所有字节复制到缓冲区中，并更新已经写入的数据的大小。
    这个方法的作用是重写了OutputStream类的write方法，实现了将一个字节数组一次性输出的功能。
     */
    public final void write(byte b[]) {
        verifyBufferSize(size + b.length);
        System.arraycopy(b, 0, buf, size, b.length);
        size += b.length;
    }

    /*
    函数public final void write(byte b[], int off, int len)是另一个公共的最终方法，用于将一个字节数组的一部分写入到缓冲区中。
    它有三个参数，即b，off和len，分别表示要写入的字节数组，起始位置和长度。
    它会先调用verifyBufferSize方法，检查缓冲区是否足够大，如果不够就扩容。
    然后，它会使用System.arraycopy方法，将b中从off开始的len个字节复制到缓冲区中，并更新已经写入的数据的大小。
    这个方法的作用是重写了OutputStream类的write方法，实现了将一个字节数组的一部分输出的功能。
     */
    public final void write(byte b[], int off, int len) {
        verifyBufferSize(size + len);
        System.arraycopy(b, off, buf, size, len);
        size += len;
    }

    /*
    根据当前网页内容，函数public final void write(int b)是又一个公共的最终方法，用于将一个字节写入到缓冲区中。
    它有一个参数，即b，表示要写入的字节。它会先调用verifyBufferSize方法，检查缓冲区是否足够大，如果不够就扩容。
    然后，它会将b转换为一个字节，存储到缓冲区中，并更新已经写入的数据的大小。
    这个方法的作用是重写了OutputStream类的write方法，实现了将一个字节输出的功能。
     */
    public final void write(int b) {
        verifyBufferSize(size + 1);
        buf[size++] = (byte) b;
    }

    public void reset() {
        size = 0;
    }

    /**
     * Returns a ByteArrayInputStream for reading back the written data
     */
    public InputStream getInputStream() {
        return new FastByteArrayInputStream(buf, size);
    }

}
