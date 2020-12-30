using Antlr4.Runtime.Misc;
using Antlr4.Runtime.Tree;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cppcx.Core.Analyzers
{
    public class FunctionDeclarationAnalyzer
    {
        private static ParameterAnalyzer _paramAnalyzer = new ParameterAnalyzer();
        private static TypedNameAnalyzer _typedNameAnalyzer = new TypedNameAnalyzer();

        [DebuggerDisplay("{ReturnType} {Name} {Parameters}")]
        public class FunctionDeclaration
        {
            public string Name { get; set; }
            public string ReturnType { get; set; }
            public List<ParameterAnalyzer.Parameter> Parameters { get; init; } = new List<ParameterAnalyzer.Parameter>();
        }

        public FunctionDeclaration Visit([NotNull] CPPCXParser.SimpleDeclarationContext context)
        {
            var retType = _typedNameAnalyzer.VisitFunctionDeclarationReturnType(context);

            var nameAndParms = context.initDeclaratorList().initDeclarator()[0].declarator().pointerDeclarator().noPointerDeclarator();

            var name = nameAndParms.noPointerDeclarator().GetText();
            var parameterDecls = nameAndParms.parametersAndQualifiers()?.parameterDeclarationClause()?.parameterDeclarationList()?.parameterDeclaration();

            var res = new FunctionDeclaration
            {
                Name = name,
                ReturnType = retType,
                Parameters = parameterDecls == null ? new List<ParameterAnalyzer.Parameter>() : _paramAnalyzer.Visit(parameterDecls),
            };

            return res;
        }
    }
}
