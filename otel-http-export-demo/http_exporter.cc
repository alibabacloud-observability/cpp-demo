// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/exporters/otlp/otlp_http_exporter_factory.h"
#include "opentelemetry/exporters/otlp/otlp_http_exporter_options.h"
#include "opentelemetry/sdk/common/global_log_handler.h"
#include "opentelemetry/sdk/resource/semantic_conventions.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/trace/provider.h"

#include <string>


namespace trace     = opentelemetry::trace;
namespace trace_sdk = opentelemetry::sdk::trace;
namespace otlp      = opentelemetry::exporter::otlp;
namespace internal_log = opentelemetry::sdk::common::internal_log;
namespace resource = opentelemetry::sdk::resource;

namespace nostd = opentelemetry::nostd;

namespace
{
    opentelemetry::exporter::otlp::OtlpHttpExporterOptions opts;
    void InitTracer()
    {
        opts.url = "<HTTP-endpoint>";

        // 创建OTLP exporter
        auto exporter  = otlp::OtlpHttpExporterFactory::Create(opts);
        auto processor = trace_sdk::SimpleSpanProcessorFactory::Create(std::move(exporter));

        resource::ResourceAttributes attributes = {
                {resource::SemanticConventions::kServiceName, "<your-service-name>"},
                {resource::SemanticConventions::kHostName, "<your-host-name>"}
        };
        auto resource = opentelemetry::sdk::resource::Resource::Create(attributes);

        std::shared_ptr<opentelemetry::trace::TracerProvider> provider =
                trace_sdk::TracerProviderFactory::Create(std::move(processor), std::move(resource));

        // 设置全局trace provider
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
}  // namespace

/*
  Usage:
  - example_otlp_http
  - example_otlp_http <URL>
  - example_otlp_http <URL> <DEBUG>
  - example_otlp_http <URL> <DEBUG> <BIN>
  <DEBUG> = yes|no, to turn console debug on or off
  <BIN> = bin, to export in binary format
*/
int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        opts.url = argv[1];
        if (argc > 2)
        {
            std::string debug  = argv[2];
            opts.console_debug = debug != "" && debug != "0" && debug != "no";
        }

        if (argc > 3)
        {
            std::string binary_mode = argv[3];
            if (binary_mode.size() >= 3 && binary_mode.substr(0, 3) == "bin")
            {
                opts.content_type = otlp::HttpRequestContentType::kBinary;
            }
        }
    }

    if (opts.console_debug)
    {
        internal_log::GlobalLogHandler::SetLogLevel(internal_log::LogLevel::Debug);
    }

    InitTracer();

    foo_library();

    CleanupTracer();
}