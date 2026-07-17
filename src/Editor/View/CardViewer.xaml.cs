using Editor.ViewModel;

namespace Editor.View;

public partial class CardViewer : ContentPage
{
	public CardViewer(CardViewerVM vm)
	{
		InitializeComponent();
		base.BindingContext = vm;
    }
	public new CardViewerVM BindingContext => (CardViewerVM)base.BindingContext;
}