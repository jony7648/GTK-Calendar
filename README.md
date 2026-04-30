<h1>GTK-Calendar</h1>

<img width="468" height="313" alt="Screenshot 2026-04-24 at 09 03 53" src="https://github.com/user-attachments/assets/b4bb375b-1265-4531-a4de-a144ed176f82" />


<h3>Overview</h3>
This application is a calendar built with the GTK framework. 
The application allows you to scroll through the various months
of the year and see which weekdays certain dates occur on.

<br>
<h3>Notes</h3>
This app also allows for the saving of notes for any date of the year.
This feature allows for users to leave notes for what happended on a certain day,
or just to have tasks planned for a future date.
<br><br><br>

<img width="300" height="313" alt="Screenshot 2026-04-24 at 11 21 56" src="https://github.com/user-attachments/assets/b428dfa6-4fbe-45f6-895d-8ccb0e872bd2" />


<br>
<h3>Themes</h3>
This calendar features both a light and a dark theme allowing users of either prefrence to enjoy using the application!
<br><br><br>

<img width="469" height="313" alt="Screenshot 2026-04-24 at 11 22 11" src="https://github.com/user-attachments/assets/b1b4ef35-705e-4905-9676-68a1bb8e1a04" />


<h3>Compilation</h3>

If you are using windows you can just download and install the precompiled build by clicking 
<a href="https://github.com/jony7648/GTK-Calendar/releases/">here</a>

but if you are using Macos or Linux, or just want to play with the source code 
you will need to compile from source.

You will first need to install the libraries required for building GTK apps, instructions can be found on the Official GTK site. After having installed the libraries required for gtk, run the following

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


