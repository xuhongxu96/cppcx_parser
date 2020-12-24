using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cppcx.Core.Models
{
    public record CxParameter(CxTypedName TypedName, string DefaultValue);
}
