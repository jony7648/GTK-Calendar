<h1>GTK-Calendar</h1>

<h3>Overview</h3>
This application is a calendar built with the GTK framework. 
The application allows you to scroll through the various months
of the year and see which weekdays certain date occur on.

<br>
<h3>Notes</h3>
This app also allows for the saving of notes for any date of the year.
This feature allows for users to leave notes for what happended on a certain day,
or just to have tasks planned for a future date.

<h3>Compilation</h3>

If you are using windows you can just download and install the precompiled build,
but if you are using Macos or Linux, or just want to play with the source code 
you will need to compile the app.

<br>

After having the libraies required for gtk setup run the following

```
git clone https://github.com/jony7648/GTK-Calendar.git
cd GTK-Calendar
mkdir build
cp -r css-styles build
cd build
cmake ..
make
./calendar
```

If you are using an IDE you can use cmake -G to generate project files
for you prefered IDE and compile the application within the IDE.


