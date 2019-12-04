# trace generated using paraview version 5.6.0
#
# To ensure correct image size when batch processing, please search
# for and uncomment the line `# renderView*.ViewSize = [*,*]`

#### import the simple module from the paraview
from paraview.simple import *
import sys

exodus_file_location = sys.argv[1]
exodus_file_name = sys.argv[2]
field_of_interest = sys.argv[3]
lower_bound = float(sys.argv[4])
upper_bound = float(sys.argv[5])
colormap_option = sys.argv[6]
if colormap_option == 'damage':
    colormap = 'jet'
if colormap_option == 'material':
    colormap = 'Rainbow Desaturated'

#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# create a new 'ExodusIIReader'
fielde = ExodusIIReader(FileName=[exodus_file_location+'/'+exodus_file_name])
fielde.PointVariables = []
fielde.NodeSetArrayStatus = []
fielde.SideSetArrayStatus = []

# get animation scene
animationScene1 = GetAnimationScene()

# update animation scene based on data timesteps
animationScene1.UpdateAnimationUsingDataTimeSteps()

# Properties modified on fielde
fielde.GenerateObjectIdCellArray = 0
fielde.GenerateGlobalElementIdArray = 0
fielde.GenerateGlobalNodeIdArray = 0
fielde.PointVariables = [field_of_interest]
fielde.ApplyDisplacements = 0
fielde.ElementBlocks = ['all']

# get active view
renderView1 = GetActiveViewOrCreate('RenderView')
# uncomment following to set a specific view size
# renderView1.ViewSize = [1612, 552]

# show data in view
fieldeDisplay = Show(fielde, renderView1)

# trace defaults for the display properties.
fieldeDisplay.Representation = 'Surface'
fieldeDisplay.ColorArrayName = [None, '']
fieldeDisplay.DiffuseColor = [1.0, 0.6666666666666666, 0.0]
fieldeDisplay.Specular = 0.3
fieldeDisplay.Ambient = 0.1
fieldeDisplay.OSPRayScaleArray = field_of_interest
fieldeDisplay.OSPRayScaleFunction = 'PiecewiseFunction'
fieldeDisplay.SelectOrientationVectors = field_of_interest
fieldeDisplay.ScaleFactor = 10.0
fieldeDisplay.SelectScaleArray = field_of_interest
fieldeDisplay.GlyphType = 'Arrow'
fieldeDisplay.GlyphTableIndexArray = field_of_interest
fieldeDisplay.GaussianRadius = 0.5
fieldeDisplay.SetScaleArray = ['POINTS', field_of_interest]
fieldeDisplay.ScaleTransferFunction = 'PiecewiseFunction'
fieldeDisplay.OpacityArray = ['POINTS', field_of_interest]
fieldeDisplay.OpacityTransferFunction = 'PiecewiseFunction'
fieldeDisplay.DataAxesGrid = 'GridAxesRepresentation'
fieldeDisplay.SelectionCellLabelFontFile = ''
fieldeDisplay.SelectionPointLabelFontFile = ''
fieldeDisplay.PolarAxes = 'PolarAxesRepresentation'
fieldeDisplay.ScalarOpacityUnitDistance = 3.910350702729909

# init the 'PiecewiseFunction' selected for 'OSPRayScaleFunction'
fieldeDisplay.OSPRayScaleFunction.Points = [0.09999999999999999, 0.0, 0.5, 0.0, 1.9, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
fieldeDisplay.ScaleTransferFunction.Points = [0.09999999999999999, 0.0, 0.5, 0.0, 1.9, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
fieldeDisplay.OpacityTransferFunction.Points = [0.09999999999999999, 0.0, 0.5, 0.0, 1.9, 1.0, 0.5, 0.0]

# init the 'GridAxesRepresentation' selected for 'DataAxesGrid'
fieldeDisplay.DataAxesGrid.XTitleFontFile = ''
fieldeDisplay.DataAxesGrid.YTitleFontFile = ''
fieldeDisplay.DataAxesGrid.ZTitleFontFile = ''
fieldeDisplay.DataAxesGrid.XLabelFontFile = ''
fieldeDisplay.DataAxesGrid.YLabelFontFile = ''
fieldeDisplay.DataAxesGrid.ZLabelFontFile = ''

# init the 'PolarAxesRepresentation' selected for 'PolarAxes'
fieldeDisplay.PolarAxes.PolarAxisTitleFontFile = ''
fieldeDisplay.PolarAxes.PolarAxisLabelFontFile = ''
fieldeDisplay.PolarAxes.LastRadialAxisTextFontFile = ''
fieldeDisplay.PolarAxes.SecondaryRadialAxesTextFontFile = ''

# reset view to fit data
renderView1.ResetCamera()

#changing interaction mode based on data extents
renderView1.InteractionMode = '2D'
renderView1.CameraPosition = [50.0, 50.0, 10000.0]
renderView1.CameraFocalPoint = [50.0, 50.0, 0.0]

# get the material library
materialLibrary1 = GetMaterialLibrary()

# update the view to ensure updated data information
renderView1.Update()

# set scalar coloring
ColorBy(fieldeDisplay, ('FIELD', 'vtkBlockColors'))

# show color bar/color legend
fieldeDisplay.SetScalarBarVisibility(renderView1, True)

# get color transfer function/color map for 'vtkBlockColors'
vtkBlockColorsLUT = GetColorTransferFunction('vtkBlockColors')
vtkBlockColorsLUT.InterpretValuesAsCategories = 1
vtkBlockColorsLUT.AnnotationsInitialized = 1
vtkBlockColorsLUT.RGBPoints = [0.09999999999999999, 0.0, 0.0, 0.5625, 0.2999998, 0.0, 0.0, 1.0, 0.7571431, 0.0, 1.0, 1.0, 0.9857142999999998, 0.5, 1.0, 0.5, 1.2142855, 1.0, 1.0, 0.0, 1.6714288, 1.0, 0.0, 0.0, 1.9, 0.5, 0.0, 0.0]
vtkBlockColorsLUT.ColorSpace = 'RGB'
vtkBlockColorsLUT.Annotations = ['0', '0', '1', '1', '2', '2', '3', '3', '4', '4', '5', '5', '6', '6', '7', '7', '8', '8', '9', '9', '10', '10', '11', '11']
vtkBlockColorsLUT.ActiveAnnotatedValues = ['0']
vtkBlockColorsLUT.IndexedColors = [1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.63, 0.63, 1.0, 0.67, 0.5, 0.33, 1.0, 0.5, 0.75, 0.53, 0.35, 0.7, 1.0, 0.75, 0.5]

# get opacity transfer function/opacity map for 'vtkBlockColors'
vtkBlockColorsPWF = GetOpacityTransferFunction('vtkBlockColors')
vtkBlockColorsPWF.Points = [0.09999999999999999, 0.0, 0.5, 0.0, 1.9, 1.0, 0.5, 0.0]

# Properties modified on vtkBlockColorsLUT
vtkBlockColorsLUT.IndexedColors = [1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.6299992370489051, 0.6299992370489051, 1.0, 0.6699931334401464, 0.5000076295109483, 0.3300068665598535, 1.0, 0.5000076295109483, 0.7499961852445258, 0.5300068665598535, 0.3499961852445258, 0.7000076295109483, 1.0, 0.7499961852445258, 0.5000076295109483]
vtkBlockColorsLUT.IndexedOpacities = [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]

animationScene1.GoToLast()

# set scalar coloring
ColorBy(fieldeDisplay, ('POINTS', field_of_interest))

# Hide the scalar bar for this color map if no visible data is colored by it.
HideScalarBarIfNotNeeded(vtkBlockColorsLUT, renderView1)

# rescale color and/or opacity maps used to include current data range
fieldeDisplay.RescaleTransferFunctionToDataRange(True, False)

# show color bar/color legend
fieldeDisplay.SetScalarBarVisibility(renderView1, True)

# get color transfer function/color map for field_of_interest
gcLUT = GetColorTransferFunction(field_of_interest)
gcLUT.RGBPoints = [0.0007144730022485157, 0.0, 0.0, 0.5625, 0.0007322421226673432, 0.0, 0.0, 1.0, 0.0007728573350143564, 0.0, 1.0, 1.0, 0.0007931649012073021, 0.5, 1.0, 0.5, 0.0008134724674002478, 1.0, 1.0, 0.0, 0.000854087679747261, 1.0, 0.0, 0.0, 0.0008743952459402067, 0.5, 0.0, 0.0]
gcLUT.ColorSpace = 'RGB'
gcLUT.ScalarRangeInitialized = 1.0

# get opacity transfer function/opacity map for field_of_interest
gcPWF = GetOpacityTransferFunction(field_of_interest)
gcPWF.Points = [0.0007144730022485157, 0.0, 0.5, 0.0, 0.0008743952459402067, 1.0, 0.5, 0.0]
gcPWF.ScalarRangeInitialized = 1

# hide color bar/color legend
fieldeDisplay.SetScalarBarVisibility(renderView1, False)

# Properties modified on renderView1
renderView1.OrientationAxesVisibility = 0

# Rescale transfer function
gcLUT.RescaleTransferFunction(lower_bound, upper_bound)

# Rescale transfer function
gcPWF.RescaleTransferFunction(lower_bound, upper_bound)

# show color bar/color legend
fieldeDisplay.SetScalarBarVisibility(renderView1, True)

# hide color bar/color legend
fieldeDisplay.SetScalarBarVisibility(renderView1, False)

# Apply a preset using its name. Note this may not work as expected when presets have duplicate names.
gcLUT.ApplyPreset(colormap, True)

# show color bar/color legend
fieldeDisplay.SetScalarBarVisibility(renderView1, True)

# hide color bar/color legend
fieldeDisplay.SetScalarBarVisibility(renderView1, False)

# current camera placement for renderView1
renderView1.InteractionMode = '2D'
renderView1.CameraPosition = [50.0, 50.0, 10000.0]
renderView1.CameraFocalPoint = [50.0, 50.0, 0.0]
renderView1.CameraParallelScale = 58.43857695756591

# save screenshot
SaveScreenshot(exodus_file_location+'/'+field_of_interest+'_'+exodus_file_location+'.png', renderView1, ImageResolution=[300, 300],
    # PNG options
    CompressionLevel='9')

#### saving camera placements for all active views

# current camera placement for renderView1
renderView1.InteractionMode = '2D'
renderView1.CameraPosition = [50.0, 50.0, 10000.0]
renderView1.CameraFocalPoint = [50.0, 50.0, 0.0]
renderView1.CameraParallelScale = 58.43857695756591

#### uncomment the following to render all views
# RenderAllViews()
# alternatively, if you want to write images, you can use SaveScreenshot(...).
