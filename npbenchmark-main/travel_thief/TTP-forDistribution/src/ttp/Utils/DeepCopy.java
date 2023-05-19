/*
这个文件是一个Java类，它定义了一个名为DeepCopy的工具类，用于实现对象的深拷贝。
深拷贝是指将一个对象的所有字段和引用的对象都复制一份，而不是只复制引用。这样可以避免修改原对象时影响到复制的对象。

这个类有一个静态方法copy，它接受一个Object类型的参数，返回一个Object类型的结果。
这个方法的实现是通过序列化和反序列化的方式来进行深拷贝。
首先，它创建了一个FastByteArrayOutputStream对象，然后用ObjectOutputStream将参数对象写入到字节数组中。
然后，它用ObjectInputStream从字节数组中读取一个新的对象，并返回。这个方法要求参数对象实现了Serializable接口，否则会抛出IOException或ClassNotFoundException异常。

这个类还引入了一个FastByteArrayOutputStream类，它是一个继承自ByteArrayOutputStream的子类，用于提高序列化和反序列化的性能。
它重写了getInputStream方法，返回一个内部类FastByteArrayInputStream的实例，而不是创建一个新的ByteArrayInputStream对象。
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
import java.io.IOException;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.ObjectOutputStream;
import java.io.ObjectInputStream;

/**
 * Utility for making deep copies (vs. clone()'s shallow copies) of
 * objects. Objects are first serialized and then deserialized. Error
 * checking is fairly minimal in this implementation. If an object is
 * encountered that cannot be serialized (or that references an object
 * that cannot be serialized) an error is printed to System.err and
 * null is returned. Depending on your specific application, it might
 * make more sense to have copy(...) re-throw the exception.
 */
public class DeepCopy {

    /**
     * Returns a copy of the object, or null if the object cannot
     * be serialized.
     */
    /*
    这个函数是一个工具方法，用于实现对象的深拷贝。
    深拷贝是指将一个对象的所有字段和引用的对象都复制一份，而不是只复制引用。这样可以避免修改原对象时影响到复制的对象。

    这个函数的参数是一个Object类型的对象，返回值也是一个Object类型的对象。
    这个函数的实现是通过序列化和反序列化的方式来进行深拷贝。
    首先，它创建了一个ByteArrayOutputStream对象，然后用ObjectOutputStream将参数对象写入到字节数组中。
    然后，它用ObjectInputStream从字节数组中读取一个新的对象，并返回。
    这个函数要求参数对象实现了Serializable接口，否则会抛出IOException或ClassNotFoundException异常。
     */
    public static Object copy(Object orig) {
        Object obj = null;
        try {
            // Write the object out to a byte array
            FastByteArrayOutputStream fbos =
                    new FastByteArrayOutputStream();
            ObjectOutputStream out = new ObjectOutputStream(fbos);
            out.writeObject(orig);
            out.flush();
            out.close();

            // Retrieve an input stream from the byte array and read
            // a copy of the object back in.
            ObjectInputStream in =
                new ObjectInputStream(fbos.getInputStream());
            obj = in.readObject();
        }
        catch(IOException e) {
            e.printStackTrace();
        }
        catch(ClassNotFoundException cnfe) {
            cnfe.printStackTrace();
        }
        return obj;
    }

}
