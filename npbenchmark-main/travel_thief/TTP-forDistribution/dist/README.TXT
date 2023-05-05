========================
BUILD OUTPUT DESCRIPTION
========================

When you build an Java application project that has a main class, the IDE
automatically copies all of the JAR
files on the projects classpath to your projects dist/lib folder. The IDE
also adds each of the JAR files to the Class-Path element in the application
JAR files manifest file (MANIFEST.MF).

To run the project from the command line, go to the dist folder and
type the following:

java -jar "TTP-forDistribution.jar" 

To distribute this project, zip up the dist folder (including the lib folder)
and distribute the ZIP file.

Notes:

* If two JAR files on the project classpath have the same name, only the first
JAR file is copied to the lib folder.
* Only JAR files are copied to the lib folder.
If the classpath contains other types of files or folders, these files (folders)
are not copied.
* If a library on the projects classpath also has a Class-Path element
specified in the manifest,the content of the Class-Path element has to be on
the projects runtime path.
* To set a main class in a standard Java project, right-click the project node
in the Projects window and choose Properties. Then click Run and enter the
class name in the Main Class field. Alternatively, you can manually type the
class name in the manifest Main-Class element.

========================
构建输出描述
========================

当您构建一个带有主类的Java应用程序项目时，IDE会自动将项目类路径上的所有JAR文件复制到项目的`dist/lib`文件夹中。IDE还会将每个JAR文件添加到应用程序JAR文件的清单文件（`MANIFEST.MF`）中的Class-Path元素中。

要从命令行运行该项目，请转到`dist`文件夹并键入以下命令：

java -jar "TTP-forDistribution.jar"

要分发此项目，请将`dist`文件夹（包括`lib`文件夹）压缩成ZIP文件并分发。

注意事项：

* 如果项目类路径上有两个同名的JAR文件，只会复制第一个JAR文件到`lib`文件夹中。
* 只有JAR文件会被复制到`lib`文件夹中。如果类路径包含其他类型的文件或文件夹，这些文件（文件夹）将不会被复制。
* 如果项目类路径上的库也在清单文件中指定了Class-Path元素，则Class-Path元素的内容必须在项目的运行时路径上。
* 要在标准Java项目中设置主类，请在项目窗口中右键单击项目节点，然后选择“属性”。然后点击“运行”并在“主类”字段中输入类名。或者，您也可以在清单文件的Main-Class元素中手动输入类名。

