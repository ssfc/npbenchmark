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

    /**
     * Constructs a stream with buffer capacity size 5K
     */
    public FastByteArrayOutputStream() {
        this(5 * 1024);
    }

    /**
     * Constructs a stream with the given initial size
     */
    public FastByteArrayOutputStream(int initSize) {
        this.size = 0;
        this.buf = new byte[initSize];
    }

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

    public int getSize() {
        return size;
    }

    /**
     * Returns the byte array containing the written data. Note that this
     * array will almost always be larger than the amount of data actually
     * written.
     */
    public byte[] getByteArray() {
        return buf;
    }

    public final void write(byte b[]) {
        verifyBufferSize(size + b.length);
        System.arraycopy(b, 0, buf, size, b.length);
        size += b.length;
    }

    public final void write(byte b[], int off, int len) {
        verifyBufferSize(size + len);
        System.arraycopy(b, off, buf, size, len);
        size += len;
    }

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
