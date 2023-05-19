/*
当前网页文本是一个Java代码文件，定义了一个名为FastByteArrayInputStream的类，它是InputStream的一个子类。
这个类用于从一个字节数组中读取数据，而不需要同步方法。它有以下属性和方法：

一个受保护的字节数组buf，用于存储数据。
一个受保护的整数count，用于记录可以从缓冲区中读取的字节数。
一个受保护的整数pos，用于记录已经从缓冲区中读取的字节数。
一个构造方法，接受一个字节数组和一个整数作为参数，用于初始化buf和count。
一个重写的available方法，返回缓冲区中剩余的可读字节数。
一个重写的read方法，返回缓冲区中下一个字节的值，如果已经到达缓冲区的末尾，则返回-1。
一个重写的read方法，接受一个字节数组，一个偏移量和一个长度作为参数，用于将缓冲区中的数据复制到给定的字节数组中，并返回实际复制的字节数，如果已经到达缓冲区的末尾，则返回-1。
一个重写的skip方法，接受一个长整数作为参数，用于跳过缓冲区中的指定数量的字节，并返回实际跳过的字节数。
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
import java.io.InputStream;
import java.io.IOException;

/**
 * ByteArrayInputStream implementation that does not synchronize methods.
 */
public class FastByteArrayInputStream extends InputStream {
    /**
     * Our byte buffer
     */
    protected byte[] buf = null;

    /**
     * Number of bytes that we can read from the buffer
     */
    protected int count = 0;

    /**
     * Number of bytes that have been read from the buffer
     */
    protected int pos = 0;

    /*
    FastByteArrayInputStream类的构造方法，它用于创建一个FastByteArrayInputStream对象。
    它接受两个参数：一个字节数组buf和一个整数count。它将这两个参数赋值给对象的属性buf和count，分别表示要读取的数据和数据的长度。
     */
    public FastByteArrayInputStream(byte[] buf, int count) {
        this.buf = buf;
        this.count = count;
    }

    /*
    用于返回缓冲区中剩余的可读字节数。它没有参数，它返回一个整数，表示缓冲区中的字节数减去已经读取的字节数。
     */
    public final int available() {
        return count - pos;
    }

    /*
    用于从缓冲区中读取一个字节的数据，并返回它的值。
    它没有参数，它返回一个整数，表示缓冲区中下一个字节的值，如果已经到达缓冲区的末尾，则返回-1。
     */
    public final int read() {
        return (pos < count) ? (buf[pos++] & 0xff) : -1;
    }

    /*
    用于将缓冲区中的数据复制到给定的字节数组中，并返回实际复制的字节数。
    它接受三个参数：一个字节数组b，一个偏移量off和一个长度len。
    它将缓冲区中从当前位置开始的len个字节的数据复制到b数组中，从off位置开始存放，并将对象的pos属性增加len。
    如果缓冲区中的可读字节数小于len，则只复制可读的字节数，并返回这个数值。如果缓冲区中没有可读的字节，则返回-1。
     */
    public final int read(byte[] b, int off, int len) {
        if (pos >= count)
            return -1;

        if ((pos + len) > count)
            len = (count - pos);

        System.arraycopy(buf, pos, b, off, len);
        pos += len;
        return len;
    }

    public final long skip(long n) {
        if ((pos + n) > count)
            n = count - pos;
        if (n < 0)
            return 0;
        pos += n;
        return n;
    }

}
