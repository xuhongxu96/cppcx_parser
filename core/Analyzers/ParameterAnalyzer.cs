using Antlr4.Runtime.Misc;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cppcx.Core.Analyzers
{
    public class ParameterAnalyzer
    {
        private static TypedNameAnalyzer _analyzer = new TypedNameAnalyzer();

        [DebuggerDisplay("{TypedName} = {DefaultValue}")]
        public class Parameter
        {
            public TypedNameAnalyzer.TypedName TypedName { get; set; }
            public string DefaultValue { get; set; }
        }

        private Parameter VisitParameterDeclaration([NotNull] CPPCXParser.ParameterDeclarationContext context)
        {
            return new Parameter { TypedName = _analyzer.Visit(context.children) };
        }

        public List<Parameter> Visit([NotNull] IEnumerable<CPPCXParser.ParameterDeclarationContext> contexts)
        {
            var res = new List<Parameter>();
            foreach (var parameterDecl in contexts)
            {
                res.Add(VisitParameterDeclaration(parameterDecl));
            }

            return res;
        }
    }
}
