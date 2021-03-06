@class usr::plt::Pad1D

This class aims for a uniform plotting interface for both `ROOT` and `ROOFIT` objects, assisting with plotting objects according to CMS recommended plotting style, removing the use of 'magic strings' needed for object plotting, reducing the verbosity of the code needed for generating publication ready plots, and improving the readability of the plotting related codes in general.

# Plotting ROOT and ROOFIT under the same interface.

`ROOT` and `RooFit` takes two distinct style of plotting: In `ROOT`, the data containers and the plotting objects are one and the same (i.e. a `TH1D` instance is used for data interaction via Fill functions, and stylising the plotting output via the `SetLineColor` and such functions); In `RooFit`, the data containers (`RooDataSet`s, `RooPdf`s) are used to generated plotting classes (TGraphs) via the plotOn function, with the assistance of a `RooPlot` object, where the generated objects are destroyed when the `RooPlot` object goes out of scope.

`RooFit` has a very good reason for doing such a thing: the process of generating a simple plotting object from a complicated, multi-variable entity is non-trivial, and in the case of serious numerical analysis, the same data entity might be required to be plotted multiple times with different extraction. Defining the plotting styles in the objects could very likely cause unwanted behaviours in the resulting plot. However, ROOT objects are relatively simple, with very few ways of presenting the stored data, so not using an additional generated object to plot and object 'makes sense'. The final design decision was to keep the plotting of `ROOT` objects plotting `ROOT`-like; and `RooFit` objects `RooFit` like:

-   `ROOT` objects are Drawn on the Pad instance, with the objects ownership untouched. A wrapper for the `PadBase::PlotObj()` functions would be used for additional style handling.
-   `RooFit` object would have their representative `TGraphs` objects generated by an internal `RooPlot` object, the then the `TGraph` have their plotting options passed onto as to the ROOT object plotting functions. This is to avoid repeated calls to the `RooPlot::Draw()` functions, as this function basically loops overall the internal objects and passes it to the working `TPad`. The `RooFit` plotting functions would also return a reference to the generated `TGraph` object to allow for additional stylising.

The plotting functions will essentially be a improved `RooFit` plotting interface using the RooCmdArg class to change the behaviour of the plotting:

    pad.Plot<ObjType>( PlotObj, Cmd1, Cmd2, Cmd3,... <arbitrary many arguments> )

For the case of plotting `RooFit` objects, all `RooFit` `RooCmdArg`s are acceptable, with some custom `RooCmdArg`s for additional enhancements.

# Object plotting behaviour enhancements

A few addition plotting behaviour for objects are present:

-   `PlotType` allows for the specification of how the data should be presented: whether as scatter plot (data point with error bars), a function line with uncertainty band or standard solid histograms or histogram error boxes. Additional parsing for setting the styling of with CMS plotting conventions (no x error bars should be present for equal-bin-width histograms...etc) are also present.

-   `TrackY` tells the Pad whether for not to adjust the Y axis range depending on input object. If it is not specified, it is automatically determined as followed:

    -   If the object is a histogram, then only the maximum value is tracked for axis range adjustment (the default 'minimum value' is 0.3).
    -   If the object is a `TGraph`, and no histogram is already present in the pad, then both the maximum and minimum values are tracked. If any histogram is already present however, the new `TGraph` would not change the axis range.

-   `EntryText` tells the object to be added to the legend on the pad. What is to be presented in the legend would be automatically determined by the `PlotType` passed to the plot function.

# Pad plotting behaviour enhancements:

-   Standard CMS and luminosity labels text would be added to the pad via the `Pad1D::DrawCMSLabel()` and `Pad1D::DrawLuminosity()` functions. The CMS label would be placed on the top-left side inside the axis frame, while the luminosity label would be placed on the top-right side outside the axis frame.
-   If a `EntryText` is specifed at any point in the pad's lifetime, A `TLegend` would be spawn on the top-right of the pad inside the axis frame. The size of the frame would be automatically adjusted. Based on the entry text lengths and the number of entries specified. The choice of placing the legend on the top-right is that HEP 1D plots typically exhibit long trailing tails.
