#include <iostream>

#include "opentelemetry/exporters/otlp/otlp_grpc_exporter_factory.h"
#include "opentelemetry/sdk/common/global_log_handler.h"
#include "opentelemetry/sdk/resource/semantic_conventions.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/trace/provider.h"

namespace trace     = opentelemetry::trace;
namespace trace_sdk = opentelemetry::sdk::trace;
namespace otlp      = opentelemetry::exporter::otlp;
namespace internal_log = opentelemetry::sdk::common::internal_log;
namespace resource = opentelemetry::sdk::resource;

namespace nostd = opentelemetry::nostd;

namespace
{
    opentelemetry::exporter::otlp::OtlpGrpcExporterOptions opts;
    void InitTracer()
    {
        opts.endpoint = "<gRPC-endpoint>";
        opts.metadata.insert(std::pair<std::string, std::string>("authentication","<gRPC-token>"));



        // 创建OTLP exporter
        auto exporter  = otlp::OtlpGrpcExporterFactory::Create(opts);
        auto processor = trace_sdk::SimpleSpanProcessorFactory::Create(std::move(exporter));

        resource::ResourceAttributes attributes = {
                {resource::SemanticConventions::kServiceName, "<your-service-name>"},
                {resource::SemanticConventions::kHostName, "<your-host-name>"}
        };
        auto resource = opentelemetry::sdk::resource::Resource::Create(attributes);

        std::shared_ptr<opentelemetry::trace::TracerProvider> provider =
                trace_sdk::TracerProviderFactory::Create(std::move(processor), std::move(resource));

        // 设置全局的trace provider
        trace::Provider::SetTracerProvider(provider);
    }

    void CleanupTracer()
    {
        std::shared_ptr<opentelemetry::trace::TracerProvider> none;
        trace::Provider::SetTracerProvider(none);
    }

    nostd::shared_ptr<trace::Tracer> get_tracer()
    {
        auto provider = trace::Provider::GetTracerProvider();
        return provider->GetTracer("library name to trace", OPENTELEMETRY_SDK_VERSION);
    }

    void f1()
    {
        auto scoped_span = trace::Scope(get_tracer()->StartSpan("f1"));
    }

    void f2()
    {
        auto scoped_span = trace::Scope(get_tracer()->StartSpan("f2"));

        f1();
        f1();
    }

    void foo_library()
    {
        auto scoped_span = trace::Scope(get_tracer()->StartSpan("library"));

        f2();
    }
}  


int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        opts.endpoint = argv[1];
        if (argc > 2)
        {
            opts.use_ssl_credentials         = true;
            opts.ssl_credentials_cacert_path = argv[2];
        }
    }
    
    InitTracer();

    foo_library();

    CleanupTracer();
}