using Editor.ViewModel;

namespace Editor.View;

public partial class SaveViewer : ContentPage
{
	public SaveViewer(SaveViewerVM vm)
	{
		InitializeComponent();
		base.BindingContext = vm;
	}
	public new SaveViewerVM BindingContext => (SaveViewerVM)base.BindingContext;
}