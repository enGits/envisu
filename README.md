enVisu
======

enVisu is a VTK front end which can easily be extended to fit your requirements.

It has been ported from an old piece of code which, at the time, was used to visualise data from
our finite volume code MOUSE. Hence enVisu still relies on the old Qt3 canvas framework. It does,
however, compile without problem under Qt-4.8; porting it to the new graphics-view framework 
should (hopefully) not be too difficult.

The main idea behind was that the software can easily be extended with custom readers and VTK
functionality. Just have a look at it and let us know what you think.

Of course the code has loads of flaws and things which don't work properly at the moment. We do
plan, however, to start using it for our own visualisation tasks from now on. So expect it to
become more usable in the near future.
