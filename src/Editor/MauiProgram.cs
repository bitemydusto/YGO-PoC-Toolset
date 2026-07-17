using Microsoft.Extensions.Logging;
using CommunityToolkit.Maui;
using Editor.ViewModel;
using Editor.View;
using PoCTools.Decoder;
using PoCTools.Library;

namespace Editor
{
    public static class MauiProgram
    {
        public static MauiApp CreateMauiApp()
        {
            var builder = MauiApp.CreateBuilder();
            builder
                .UseMauiApp<App>()
                .UseMauiCommunityToolkit()
                .ConfigureFonts(fonts =>
                {
                    fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular");
                    fonts.AddFont("OpenSans-Semibold.ttf", "OpenSansSemibold");
                });

            builder.Services.AddSingleton<IPoCLibrary, PoCLibrary>();
            builder.Services.AddSingleton<IPoCDecoder, PoCDecoder>();


            builder.Services.AddSingleton<CardViewerVM>();
            builder.Services.AddSingleton<CardViewer>();
            builder.Services.AddSingleton<SaveViewerVM>();
            builder.Services.AddSingleton<SaveViewer>();

#if DEBUG
            builder.Logging.AddDebug();
#endif

            return builder.Build();
        }
    }
}
