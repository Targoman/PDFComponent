################################################################################
#   Copyright 2018 by Targoman.com
#
#   Tarjomyar is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Lesser General Public License for more details.
#
#   You should have received a copy of the GNU Lesser General Public License
#   along with Targoman. If not, see <http://www.gnu.org/licenses/>.
################################################################################

TEMPLATE = subdirs

include(../Project.pri)

SUBDIRS += fpdfapi fpdfdoc fpdftext fxcodec fxcrt fxge
CONFIG += ordered

fpdfapi.file = ./fpdfapi.pro
fpdfdoc.file = ./fpdfdoc.pro
fpdftext.file = ./fpdftext.pro
fxcodec.file = ./fxcodec.pro
fxcrt.file = ./fxcrt.pro
fxge.file = ./fxge.pro
