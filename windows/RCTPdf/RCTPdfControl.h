#pragma once

#include <vector>
#include "winrt/Windows.UI.Xaml.h"
#include "winrt/Windows.UI.Xaml.Markup.h"
#include "winrt/Windows.UI.Xaml.Interop.h"
#include "winrt/Windows.UI.Xaml.Controls.Primitives.h"
#include "winrt/Microsoft.ReactNative.h"
#include "NativeModules.h"
#include "RCTPdfControl.g.h"

namespace winrt::RCTPdf::implementation
{
    struct PDFPageInfo {
      PDFPageInfo(winrt::Windows::UI::Xaml::Controls::Image image, winrt::Windows::Data::Pdf::PdfPage page, double imageScale, double renderScale);
      PDFPageInfo(const PDFPageInfo&);
      PDFPageInfo(PDFPageInfo&&);
      unsigned pageVisiblePixels(bool horizontal, double viewportStart, double viewportEnd) const;
      unsigned pageSize(bool horizontal) const;
      bool needsRender() const;
      winrt::IAsyncAction render();
      winrt::IAsyncAction render(double useScale);
      unsigned height, width;
      unsigned scaledHeight, scaledWidth;
      unsigned scaledTopOffset, scaledLeftOffset;
      double imageScale; // scale at which the image is displayed
      // Multiple taks can update the image, use the render scale as the sync point
      std::atomic<double> renderScale; // scale at which the image is rendered
      winrt::Windows::UI::Xaml::Controls::Image image;
      winrt::Windows::Data::Pdf::PdfPage page;
    };

    struct RCTPdfControl : RCTPdfControlT<RCTPdfControl>
    {
    public:
        RCTPdfControl(Microsoft::ReactNative::IReactContext const& reactContext);

        static winrt::Windows::Foundation::Collections::
          IMapView<winrt::hstring, winrt::Microsoft::ReactNative::ViewManagerPropertyType>
          NativeProps() noexcept;
        void UpdateProperties(winrt::Microsoft::ReactNative::IJSValueReader const& propertyMapReader) noexcept;

        static winrt::Microsoft::ReactNative::ConstantProviderDelegate
          ExportedCustomBubblingEventTypeConstants() noexcept;
        static winrt::Microsoft::ReactNative::ConstantProviderDelegate
          ExportedCustomDirectEventTypeConstants() noexcept;

        static winrt::Windows::Foundation::Collections::IVectorView<winrt::hstring> Commands() noexcept;
        void DispatchCommand(
          winrt::hstring const& commandId,
          winrt::Microsoft::ReactNative::IJSValueReader const& commandArgsReader) noexcept;

        void PagesContainer_PointerWheelChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e);
    private:
        Microsoft::ReactNative::IReactContext m_reactContext{ nullptr };

        // Global lock to access the data stuff
        // the pages are rendered in an async way
        std::shared_mutex m_rwlock;
        // URI and password of the PDF
        std::string m_pdfURI;
        std::string m_pdfPassword;
        // Current active page
        int m_currentPage = 0;
        // Margins of each page
        int m_margins = 10;
        // Scale at which the PDF is displayed
        double m_scale = 0.2;
        double m_minScale = 0.1;
        double m_maxScale = 3.0;
        // Are we in "horizontal" mode?
        bool m_horizontal = false;
        // Render the pages in reverse order
        bool m_reverse = false;

        double m_targetHorizontalOffset = -1;
        double m_targetVerticalOffset = -1;
        std::vector<PDFPageInfo> m_pages;
        void UpdatePagesInfoMarginOrScale();

        winrt::fire_and_forget OnViewChanged(winrt::Windows::Foundation::IInspectable const& sender,
          winrt::Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs const& args);
        winrt::Windows::UI::Xaml::Controls::ScrollViewer::ViewChanged_revoker m_viewChangedRevoker{};

        winrt::fire_and_forget LoadPDF(std::unique_lock<std::shared_mutex> lock, int fitPolicy, bool singlePage);
        void GoToPage(int page);
        void Rescale(double newScale, double newMargin, bool goToNewPosition);
        void SignalError(const std::string& error);
        void SignalLoadComplete(int totalPages, int width, int height);
        void SignalPageChange(int page, int totalPages);
        void SignalScaleChanged(double scale);
    };
}

namespace winrt::RCTPdf::factory_implementation
{
    struct RCTPdfControl : RCTPdfControlT<RCTPdfControl, implementation::RCTPdfControl>
    {
    };
}
