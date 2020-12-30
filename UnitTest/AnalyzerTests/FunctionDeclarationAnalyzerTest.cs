using cppcx.Core.Analyzers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnitTest.Utils;
using Xunit;
using Xunit.Abstractions;

namespace UnitTest.AnalyzerTests
{
    public class FunctionDeclarationAnalyzerTest
    {
        private readonly ITestOutputHelper _output;

        public FunctionDeclarationAnalyzerTest(ITestOutputHelper output)
        {
            _output = output;
        }

        private CPPCXParser.SimpleDeclarationContext GetTestFunctionDeclarationFor(string filename, int option)
        {
            var tu = CxParser.Parse(filename);
            switch ($"{filename} - {option}")
            {
                case "data/App.xaml.h - 0":
                    // void f();
                    return tu.declarationseq().declaration()[0].namespaceDefinition().declarationseq().declaration()[0].namespaceDefinition().declarationseq().declaration()[3].blockDeclaration().simpleDeclaration();
                case "data/App.xaml.h - 1":
                    // void g(int a);
                    return tu.declarationseq().declaration()[0].namespaceDefinition().declarationseq().declaration()[0].namespaceDefinition().declarationseq().declaration()[4].blockDeclaration().simpleDeclaration();
                case "data/App.xaml.h - 2":
                    return tu.declarationseq().declaration()[0].namespaceDefinition().declarationseq().declaration()[1].namespaceDefinition().declarationseq().declaration()[0].blockDeclaration().simpleDeclaration();
                default:
                    throw new NotImplementedException();
            }
        }

        [Theory]
        [InlineData("data/App.xaml.h", 0, "void", "f")]
        [InlineData("data/App.xaml.h", 1, "void", "g", "int a")]
        [InlineData("data/App.xaml.h", 2, "Windows::ApplicationModel::Activation::LaunchActivatedEventArgs", "h", "int a", "Platform::StringReference b")]
        public void Test(string filename, int option, string retType, string funcName, params string[] parameters)
        {
            var ctx = GetTestFunctionDeclarationFor(filename, option);
            var analyzer = new FunctionDeclarationAnalyzer();
            var res = analyzer.Visit(ctx);

            Assert.Equal(retType, res.ReturnType);
            Assert.Equal(funcName, res.Name);
            Assert.Equal(parameters, res.Parameters.Select(o => $"{o.TypedName.Type} {o.TypedName.Name}" + (o.DefaultValue == null ? "" : $" = {o.DefaultValue}")));
        }

        [Fact]
        public void TestRatpack()
        {
            var tu = CxParser.Parse("data/ratpack.h");

            var decls = new NamespaceBodyAnalyzer().Visit(tu.GetChild(0)).FunctionDeclarations;

            var analyzer = new FunctionDeclarationAnalyzer();
            foreach (var decl in decls)
            {
                var res = analyzer.Visit(decl.Context);
                var paramStr = string.Join(", ", res.Parameters.Select(o => $"{ConvertType(o.TypedName.Type, o.TypedName.Name)} {o.TypedName.Name}"));
                _output.WriteLine($"[DllImport(DllName, EntryPoint=\"{res.Name}\")]");
                _output.WriteLine($"public static extern {ConvertType(res.ReturnType, null)} {ConvertFuncName(res.Name)} ( {paramStr} );");
            }
        }

        private string ConvertFuncName(string name)
        {
            if (name.StartsWith("ratpack_"))
                name = name.Substring("ratpack_".Length);

            var sb = new StringBuilder();
            bool newWord = true;
            for (var i = 0; i < name.Length; ++i)
            {
                if (newWord)
                {
                    newWord = false;
                    sb.Append(char.ToUpper(name[i]));
                }
                else if (name[i] == '_')
                {
                    newWord = true;
                }
                else
                {
                    sb.Append(name[i]);
                }
            }
            return sb.ToString();
        }

        private string ConvertType(string t, string name)
        {
            var refOrOut = name == "ret" ? "out" : "ref";

            switch (t.Trim())
            {
                case "uint32_t *":
                    return $"{refOrOut} uint";
                case "int32_t *":
                    return $"{refOrOut} int";
                case "int64_t *":
                    return $"{refOrOut} long";
                case "uint64_t *":
                    return $"{refOrOut} ulong";
                case "ratpack_string_t * *":
                    return $"{refOrOut} StrPtr";
                case "const ratpack_number_t * const":
                    return "NumberPtr";
                case "const ratpack_rational_t * const":
                    return "RationalPtr";
                case "uint32_t":
                    return "uint";
                case "int32_t":
                    return "int";
                case "int64_t":
                    return "long";
                case "uint64_t":
                    return "ulong";
                case "wchar_t":
                    return "char";
                case "ratpack_string_t *":
                    return "StrPtr";
                case "const ratpack_string_t *":
                    return "StrPtr";
                case "CALC_EXPORT ratpack_error_code_t":
                    return "ErrorCodeType";
                case "ratpack_rational_t * *":
                    return $"{refOrOut} RationalPtr";
                case "ratpack_rational_t *":
                    return "RationalPtr";
                case "ratpack_number_t *":
                    return "NumberPtr";
                case "ratpack_number_t * *":
                    return $"{refOrOut} NumberPtr";
                case "const ratpack_rational_t *":
                    return "RationalPtr";
                case "const ratpack_number_t *":
                    return "NumberPtr";
                case "ratpack_number_format_type":
                    return "NumberFormat";
                case "ratpack_angle_type":
                    return "AngleType";
                case "const wchar_t *":
                    return "[MarshalAs(UnmanagedType.LPWStr)] string";
                case "bool *":
                    return "out bool";

                default: return t;
            }
        }
    }
}
